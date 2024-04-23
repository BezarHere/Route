#pragma once
#include <errno.h>
#include <ostream>

namespace route
{
	enum class Error
	{
		Ok,
		Fault,

		Busy,
		Empty,
		NotEmpty,
		Full,
		NotFull,

		OutOfRange,
		InvalidIndex,
		InvalidLength,
		InvalidRange,
		InvalidKey,
		InvalidFlags,

		Overflow,
		Underflow,

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

		JustCryAboutIt,

		UNKNOWN,
	};
}

namespace std
{
	ostream &operator<<( ostream &stream, const route::Error err ) {
		constexpr char NamesTable[][ 24 ] = {
			{ "Ok" },
			{ "Fault" },
			{ "Busy" },
			{ "Empty" },
			{ "NotEmpty" },
			{ "Full" },
			{ "NotFull" },
			{ "OutOfRange" },
			{ "InvalidIndex" },
			{ "InvalidLength" },
			{ "InvalidRange" },
			{ "InvalidKey" },
			{ "InvalidFlags" },
			{ "Overflow" },
			{ "Underflow" },
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
			{ "JustCryAboutIt" },
			{ "UNKNOWN" }
		};
		return stream << NamesTable[ static_cast<int>(err) ];
	}
}
