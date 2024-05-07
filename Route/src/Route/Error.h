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

    EndOfFile,

    OutOfRange,

    InvalidValue,
    InvalidEnum,
    InvalidIndex,
    InvalidLength,
    InvalidRange,
    InvalidKey,
    InvalidFlags,
    InvalidHandle,
    InvalidID,

    InvalidOperation,
    IllegalOperation,
    OperationIncomplete,

    Overflow,
    StackOverflow,
    Underflow,
    StackUnderflow,

    MemoryError,
    OutOfMemory,
    NoAllocator,

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
    UnknownType,

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

    Expired,
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

    ConnectionExpired,
    ConnectionCanceled,
    ConnectionAborted,
    ConnectionRefused,

    AlreadyConnected,
    AlreadyConnecting,

    DataEncrypted,
    BufferEncrypted,

    HostUnreachable,

    NotAvailable,
    AlreadyAvailable,

    BufferTooSmall,

    Inaccessible,
    Unauthorized,

    ServiceUnavailable,
    ServiceLocked,
    ServiceNotInitialized,
    ServiceDisabled,
    ServiceInactive,

    ObjectIncomplete,

    DeviceUnavailable,
    DeviceInactive,
    DeviceInactive,

    DeadPointer,
    DeadData,

    JustCryAboutIt,

    UNKNOWN,
  };
}

namespace std
{
  inline ostream &operator<<(ostream &stream, const route::Error err) {
    constexpr ostream::char_type NamesTable[][24] = {
      { "Ok" },
      { "Fault" },
      { "Busy" },
      { "Empty" },
      { "NotEmpty" },
      { "Full" },
      { "NotFull" },
      { "EndOfFile" },
      { "OutOfRange" },
      { "InvalidValue" },
      { "InvalidEnum" },
      { "InvalidIndex" },
      { "InvalidLength" },
      { "InvalidRange" },
      { "InvalidKey" },
      { "InvalidFlags" },
      { "InvalidHandle" },
      { "InvalidID" },
      { "InvalidOperation" },
      { "IllegalOperation" },
      { "OperationIncomplete" },
      { "Overflow" },
      { "StackOverflow" },
      { "Underflow" },
      { "StackUnderflow" },
      { "MemoryError" },
      { "OutOfMemory" },
      { "NoAllocator" },
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
      { "UnknownType" },
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
      { "Expired" },
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
      { "ConnectionExpired" },
      { "ConnectionCanceled" },
      { "ConnectionAborted" },
      { "ConnectionRefused" },
      { "AlreadyConnected" },
      { "AlreadyConnecting" },
      { "DataEncrypted" },
      { "BufferEncrypted" },
      { "HostUnreachable" },
      { "NotAvailable" },
      { "AlreadyAvailable" },
      { "BufferTooSmall" },
      { "Inaccessible" },
      { "Unauthorized" },
      { "ServiceUnavailable" },
      { "ServiceLocked" },
      { "ServiceNotInitialized" },
      { "ServiceDisabled" },
      { "ServiceInactive" },
      { "ObjectIncomplete" },
      { "DeviceUnavailable" },
      { "DeviceInactive" },
      { "DeviceInactive" },
      { "DeadPointer" },
      { "DeadData" },
      { "JustCryAboutIt" },
      { "UNKNOWN" }
    };
    return stream << NamesTable[static_cast<size_t>(err)];
  }

  // sadly, it doesn't work :[
  //inline operator bool( const route::Error err ) {
  //	return err != route::Error::Ok;
  //}
}
