#pragma once
#include <ostream>

namespace route
{
	enum class Error
	{
		Ok,
		Fault,

		Busy,
		Empty,
		Full,

		Invalid,
		Illegal,

		TooBig,
		TooSmall,

		DoesNotExist,
		AlreadyExist,

		CanNotRead,
		CanNotWrite,

		AlreadyLocked,
		CanNotLock,
		DataLock,

		AlreadyUnlocked,
		CanNotUnlock,
		DataUnlock,

		WillDeadlock,


		FileDoesNotExist,
		FileAlreadyExist,

		NotImplemented,
		AlreadyImplemented,

		UnexpectedNullptr,
		ExpectedNullptr,

		UnexpectedValue,

		DataIncomplete,
		DataInvalid,
		DataInsufficient,

		TypeMismatch,
		IllegalType,

		CurrentlyInactive,
		CurrentlyDisabled,

		RequiresCacheRebuild,

		Unknown,
	};
}

namespace std
{
	ostream &operator<<( ostream &stream, const route::Error err ) {
		constexpr char NamesTable[][ 32 ] = {
			{"Ok"},
			{"Fault"},
			{"Busy"},
			{"Empty"},
			{"Full"},
			{"Invalid"},
			{"Illegal"},
			{"TooBig"},
			{"TooSmall"},
			{"DoesNotExist"},
			{"AlreadyExist"},
			{"CanNotRead"},
			{"CanNotWrite"},
			{"AlreadyLocked"},
			{"CanNotLock"},
			{"DataLock"},
			{"AlreadyUnlocked"},
			{"CanNotUnlock"},
			{"DataUnlock"},
			{"WillDeadlock"},
			{"FileDoesNotExist"},
			{"FileAlreadyExist"},
			{"NotImplemented"},
			{"AlreadyImplemented"},
			{"UnexpectedNullptr"},
			{"ExpectedNullptr"},
			{"UnexpectedValue"},
			{"DataIncomplete"},
			{"DataInvalid"},
			{"DataInsufficient"},
			{"TypeMismatch"},
			{"IllegalType"},
			{"CurrentlyInactive"},
			{"CurrentlyDisabled"},
			{"RequiresCacheRebuild"},
			{"Unknown"}
		};
		return stream << NamesTable[ static_cast<int>(err) ];
	}
}
