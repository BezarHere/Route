#pragma once
#include <ostream>

namespace route
{
  enum class Error : unsigned
  {
    Ok,
    Fault,

    Busy,
    Empty,
    NotEmpty,
    Full,
    NotFull,

    OutOfRange,

    InvalidValue,
    InvalidEnum,
    InvalidIndex,
    InvalidLength,
    InvalidRange,
    InvalidKey,
    InvalidFlags,
    InvalidOperation,

    Overflow,
    StackOverflow,
    Underflow,
    StackOverflow,

    MemoryError,
    OutOfMemory,

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

    IsDirectory,
    IsFile,

    NotImplemented,
    AlreadyImplemented,

    UnexpectedNullptr,
    ExpectedNullptr,

    UnexpectedValue,
    UnexpectedResult,

    DataIncomplete,
    DataInvalid,
    DataInsufficient,

    TypeMismatch,
    IllegalType,

    CurrentlyInactive,
    CurrentlyDisabled,

    RequiresCacheRebuild,

    BadValue,
    BadFormat,
    BadChecksum,
    BadKey,
    BadHash,
    BadMessage,

    TimedOut,
    CanNotConnect,
    Disconnected,
    Interrupted,

    WouldBlock,

    NameTooLong,

    NoCache,
    NoLock,
    NoSystem,

    OutOfDate,
    InProgress,

    NoOwner,
    NoData,
    NoName,
    NoValue,
    NoKey,
    NoBuffer,

    Canceled,
    Aborted,
    Refused,

    ConnectionCanceled,
    ConnectionAborted,
    ConnectionRefused,
    AlreadyConnected,
    AlreadyConnecting,

    HostUnreachable,

    NotAvailable,
    AlreadyAvailable,

    BufferTooSmall,

    Inaccessible,
    Unauthorized,

    ServiceUnavailable,
    ServiceLocked,
    ServiceNotInitialized,

    InvalidFramebufferOperation,
    FramebufferIncomplete,

    JustCryAboutIt,

    UNKNOWN,
  };
}

namespace std
{
  inline ostream &operator<<( ostream &stream, const route::Error err ) {
    constexpr char NamesTable[][ 32 ] = {
      { "Ok" },
      { "Fault" },
      { "Busy" },
      { "Empty" },
      { "NotEmpty" },
      { "Full" },
      { "NotFull" },
      { "OutOfRange" },
      { "InvalidValue" },
      { "InvalidEnum" },
      { "InvalidIndex" },
      { "InvalidLength" },
      { "InvalidRange" },
      { "InvalidKey" },
      { "InvalidFlags" },
      { "InvalidOperation" },
      { "Overflow" },
      { "StackOverflow" },
      { "Underflow" },
      { "StackOverflow" },
      { "MemoryError" },
      { "OutOfMemory" },
      { "Invalid" },
      { "Illegal" },
      { "TooBig" },
      { "TooSmall" },
      { "DoesNotExist" },
      { "AlreadyExist" },
      { "CanNotRead" },
      { "CanNotWrite" },
      { "AlreadyLocked" },
      { "CanNotLock" },
      { "DataLock" },
      { "AlreadyUnlocked" },
      { "CanNotUnlock" },
      { "DataUnlock" },
      { "WillDeadlock" },
      { "FileDoesNotExist" },
      { "FileAlreadyExist" },
      { "IsDirectory" },
      { "IsFile" },
      { "NotImplemented" },
      { "AlreadyImplemented" },
      { "UnexpectedNullptr" },
      { "ExpectedNullptr" },
      { "UnexpectedValue" },
      { "UnexpectedResult" },
      { "DataIncomplete" },
      { "DataInvalid" },
      { "DataInsufficient" },
      { "TypeMismatch" },
      { "IllegalType" },
      { "CurrentlyInactive" },
      { "CurrentlyDisabled" },
      { "RequiresCacheRebuild" },
      { "BadValue" },
      { "BadFormat" },
      { "BadChecksum" },
      { "BadKey" },
      { "BadHash" },
      { "BadMessage" },
      { "TimedOut" },
      { "CanNotConnect" },
      { "Disconnected" },
      { "Interrupted" },
      { "WouldBlock" },
      { "NameTooLong" },
      { "NoCache" },
      { "NoLock" },
      { "NoSystem" },
      { "OutOfDate" },
      { "InProgress" },
      { "NoOwner" },
      { "NoData" },
      { "NoName" },
      { "NoValue" },
      { "NoKey" },
      { "NoBuffer" },
      { "Canceled" },
      { "Aborted" },
      { "Refused" },
      { "ConnectionCanceled" },
      { "ConnectionAborted" },
      { "ConnectionRefused" },
      { "AlreadyConnected" },
      { "AlreadyConnecting" },
      { "HostUnreachable" },
      { "NotAvailable" },
      { "AlreadyAvailable" },
      { "BufferTooSmall" },
      { "Inaccessible" },
      { "Unauthorized" },
      { "ServiceUnavailable" },
      { "ServiceLocked" },
      { "ServiceNotInitialized" },
      { "InvalidFramebufferOperation" },
      { "FramebufferIncomplete" },
      { "JustCryAboutIt" },
      { "UNKNOWN" }
    };
    return stream << NamesTable[ static_cast<unsigned>(err) ];
  }

  // sadly, it doesn't work :[
  //inline operator bool( const route::Error err ) {
  //	return err != route::Error::Ok;
  //}
}
