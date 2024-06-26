#include <iostream>
#include <Route.h>
#include <map>

#pragma comment(lib, "Route.lib")
using namespace route;

static int sx = 42;

struct A
{
  A() : x{ new int{sx++} } {
    std::cout << "object created " << this << '\n';
  }

  A &operator=(const A &obj) {
    x = obj.x;
    std::cout << "called assigen op for " << this << '\n';
    return *this;
  }

  ~A() {
    std::cout << "object destroyed " << this << '\n';
    delete x;
  }

  void test() {
  }
  int *x;
};

struct B : public A
{
  B() {
    std::cout << "B created " << this << '\n';
  }

  ~B() {
    std::cout << "B destroyed " << this << '\n';
  }

  void test() {
    std::cout << "test called\n";
  }

  int y = -sx;
};


volatile ObjectType type = ObjectType::Object2D;
volatile float val = 0.12;
int main() {
  constexpr bool ppp = std::is_assignable_v<route::object, route::object>;

  //{
  //	imap<std::string> imap{};
  //	std::map<int, std::string> map{};

  //	imap.add( 44, "hello" );
  //	imap.add( 64, "x64" );
  //	imap.add( 86, "x86" );
  //	imap.add( 69, "nice" );
  //	imap.add( 37, "so rand" );
  //	imap.add( 42, "galaxy" );
  //	imap.add( 44 * 2, "hello" );
  //	imap.add( 64 * 2, "x64" );
  //	imap.add( 86 * 2, "x86" );
  //	imap.add( 69 * 2, "nice" );
  //	imap.add( 37 * 2, "so rand" );
  //	imap.add( 42 * 2, "galaxy" );
  //}

  Window window{ "hello", route::Rect2i(100, 100, 500, 500) };
  GraphicsDevice device{ window };

  Renderer &renderer = device.get_renderer();
  route::Scene tree{};

  for (int i = 0; i < 100; i++)
  {
    object obj((ObjectType)(i & 1));
    std::unique_ptr<Component> comp{ new ShapeComp(ShapeData(shapes::Type::Circle, 1.5f)) };
    obj.add_component(std::move(comp));
    tree.add_object(obj, npos);
  }

  Application app{ window, renderer };

  const VrtxC2 vertices[3] = {
    { {  } }
  };


  app.start();
  app.stop();

}
