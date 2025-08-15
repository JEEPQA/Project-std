#include <string>
#include <basetsd.h>
using namespace std::string_literals; // enables s-suffix for std::string literals


#pragma pack(1)

typedef struct {
    UINT32    Data1;
    UINT16    Data2;
    UINT16    Data3;
    UINT8     Data4[8];
} GUID;
typedef GUID EFI_GUID;
typedef UINT64 UINTN;

///
/// To avoid confusion in interpreting frames, the communication buffer should always
/// begin with EFI_MM_COMMUNICATE_HEADER
///
typedef struct {
    ///
    /// Allows for disambiguation of the message format.
    ///
    EFI_GUID    HeaderGuid;
    ///
    /// Describes the size of Data (in bytes) and does not include the size of the header.
    ///
    UINTN       MessageLength;
    ///
    /// Designates an array of bytes that is MessageLength in size.
    ///
    UINT8       Data[1];
} EFI_MM_COMMUNICATE_HEADER;





typedef struct DirectMsgArgs {
    /// Implementation define argument 0, this will be set to/from x3(v1) or x4(v2)
    UINTN    Arg0;

    /// Implementation define argument 1, this will be set to/from x4(v1) or x5(v2)
    UINTN    Arg1;

    /// Implementation define argument 2, this will be set to/from x5(v1) or x6(v2)
    UINTN    Arg2;

    /// Implementation define argument 3, this will be set to/from x6(v1) or x7(v2)
    UINTN    Arg3;

    /// Implementation define argument 4, this will be set to/from x7(v1) or x8(v2)
    UINTN    Arg4;

    /// Implementation define argument 5, this will be set to/from x9(v2)
    UINTN    Arg5;

    /// Implementation define argument 6, this will be set to/from x10(v2)
    UINTN    Arg6;

    /// Implementation define argument 7, this will be set to/from x11(v2)
    UINTN    Arg7;

    /// Implementation define argument 8, this will be set to/from x12(v2)
    UINTN    Arg8;

    /// Implementation define argument 9, this will be set to/from x13(v2)
    UINTN    Arg9;

    /// Implementation define argument 10, this will be set to/from x14(v2)
    UINTN    Arg10;

    /// Implementation define argument 11, this will be set to/from x15(v2)
    UINTN    Arg11;

    /// Implementation define argument 12, this will be set to/from x16(v2)
    UINTN    Arg12;

    /// Implementation define argument 13, this will be set to/from x17(v2)
    UINTN    Arg13;
} DIRECT_MSG_ARGS;

/** MmCommunication Header for Misc service.
    Misc service doesn't use MmCommunication Buffer.
    This structure is used to dispatch Misc services by StandaloneMm.
 */
typedef struct {
    /// Service guid
    EFI_GUID           HeaderGuid;

    /// Length of Message. In case of misc service, sizeof (EventSvcArgs)
    UINTN              MessageLength;

    /// Delivered register values.
    DIRECT_MSG_ARGS    DirectMsgArgs;
} MISC_MM_COMMUNICATE_BUFFER;

#pragma pack()

#define ARM_N1X
#undef ARM_N1X
int main()
{
    // Character literals
    auto c0 = 'A'; // char
    auto c1 = u8'A'; // char
    auto c2 = L'A'; // wchar_t
    auto c3 = u'A'; // char16_t
    auto c4 = U'A'; // char32_t

    // Multicharacter literals
    auto m0 = 'abcd'; // int, value 0x61626364

    // String literals
    auto s0 = "hello"; // const char*
    auto s1 = u8"hello"; // const char* before C++20, encoded as UTF-8,
    // const char8_t* in C++20
    auto s2 = L"hello"; // const wchar_t*
    auto s3 = u"hello"; // const char16_t*, encoded as UTF-16
    auto s4 = U"hello"; // const char32_t*, encoded as UTF-32

    // Raw string literals containing unescaped \ and "
    auto R0 = R"("Hello \ world")"; // const char*
    auto R1 = u8R"("Hello \ world")"; // const char* before C++20, encoded as UTF-8,
    // const char8_t* in C++20
    auto R2 = LR"("Hello \ world")"; // const wchar_t*
    auto R3 = uR"("Hello \ world")"; // const char16_t*, encoded as UTF-16
    auto R4 = UR"("Hello \ world")"; // const char32_t*, encoded as UTF-32

    // Combining string literals with standard s-suffix
    auto S0 = "hello"s; // std::string
    auto S1 = u8"hello"s; // std::string before C++20, std::u8string in C++20
    auto S2 = L"hello"s; // std::wstring
    auto S3 = u"hello"s; // std::u16string
    auto S4 = U"hello"s; // std::u32string

    // Combining raw string literals with standard s-suffix
    auto S5 = R"("Hello \ world")"s; // std::string from a raw const char*
    auto S6 = u8R"("Hello \ world")"s; // std::string from a raw const char* before C++20, encoded as UTF-8,
    // std::u8string in C++20
    auto S7 = LR"("Hello \ world")"s; // std::wstring from a raw const wchar_t*
    auto S8 = uR"("Hello \ world")"s; // std::u16string from a raw const char16_t*, encoded as UTF-16
    auto S9 = UR"("Hello \ world")"s; // std::u32string from a raw const char32_t*, encoded as UTF-32


    const char* ascii = "Hello, ASCII";  // ´«Í³µÄ char
    //const char8_t* utf8 = u8"Hello, UTF-8";  // UTF-8 ×Ö·û´®
    const char16_t* utf16 = u"Hello, UTF-16";  // UTF-16 ×Ö·û´®
    const char32_t* utf32 = U"Hello, UTF-32";  // UTF-32 ×Ö·û´®

#ifdef ARM_N1X
    int i = 1;
    auto a = "hello"s; // std::string
#endif

    printf("Size of EFI_MM_COMMUNICATE_HEADER: %d\n", sizeof(EFI_MM_COMMUNICATE_HEADER));
}