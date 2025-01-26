
//这都定义的东西
#define size_t unsigned int
#define WORD unsigned short
#define LONG long
#define DWORD unsigned long

#define BYTE unsigned char
#define NULL ((void*)0)
#define LPVOID (void*)
#define bool char
#define true 1
#define false 0
typedef int(__stdcall* FARPROC)();

//这里一定要用__stdcall 
typedef void* (__stdcall *GETPROADD)(DWORD, const char*);
typedef DWORD(__stdcall *LOADLIB)(const char*);
typedef void* (__stdcall *VIRTUALALLOC)(void*,size_t,DWORD,DWORD);
typedef bool(__stdcall *VIRTUALFREE)(void*,size_t,DWORD);
typedef DWORD(__stdcall *OPENPROCESS)(DWORD,bool,DWORD);
typedef bool(__stdcall *CLOSEHANDLE)(DWORD);
typedef DWORD(__stdcall *CREATEREMOTETHREAD)(DWORD,void*,size_t,void*,void*,DWORD,void*);
typedef DWORD(__stdcall *WAITFORSINGLEOBJECT)(DWORD,DWORD);

//全局变量都写在这里  注意  全局变量所有的值都不能为0 不然会崩溃  为什么这样 问就是懒 注入器那边可以少些很多东西
//这个结构体单纯为了和易语言进行交互的
typedef struct EProceInfo{
    DWORD EProcessID;//易语言进程id
    void* print_Buff;//输出函数缓存区地址
    void* print_funcAdd;//输出函数指针
}EProceInfo;
EProceInfo* EPI = (EProceInfo*)1;//这个一定要作为第一个全局变量 绝对不能更改 为了好定位地址

//api函数表
DWORD kernel32 = (DWORD)1;
GETPROADD GetproAdd = (GETPROADD)1;
LOADLIB LoadLib = (LOADLIB)1;
VIRTUALALLOC VirtualAlloc = (VIRTUALALLOC)1;
VIRTUALFREE VirtualFree=(VIRTUALFREE)1;
OPENPROCESS OpenProcess = (OPENPROCESS)1;
CLOSEHANDLE CloseHandle = (CLOSEHANDLE)1;
CREATEREMOTETHREAD CreateRemoteThread = (CREATEREMOTETHREAD)1;
WAITFORSINGLEOBJECT WaitForSingleObject = (WAITFORSINGLEOBJECT)1;




// 定义PE文件结构
typedef struct _IMAGE_DOS_HEADER {
    WORD e_magic;      // Magic number (MZ)
    WORD e_cblp;       // Bytes on last page of file
    WORD e_cp;         // Pages in file
    WORD e_crlc;       // Relocations
    WORD e_cparhdr;    // Size of header in paragraphs
    WORD e_minalloc;   // Minimum extra paragraphs needed
    WORD e_maxalloc;   // Maximum extra paragraphs needed
    WORD e_ss;         // Initial (relative) SS value
    WORD e_sp;         // Initial SP value
    WORD e_csum;       // Checksum
    WORD e_ip;         // Initial IP value
    WORD e_cs;         // Initial (relative) CS value
    WORD e_lfarlc;     // File address of relocation table
    WORD e_ovno;       // Overlay number
    WORD e_res[4];     // Reserved words
    WORD e_oemid;      // OEM identifier (for e_oeminfo)
    WORD e_oeminfo;    // OEM information; e_oemid specific
    WORD e_res2[10];   // Reserved words
    LONG e_lfanew;     // File address of new exe header
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
    WORD Machine;
    WORD NumberOfSections;
    DWORD TimeDateStamp;
    DWORD PointerToSymbolTable;
    DWORD NumberOfSymbols;
    WORD SizeOfOptionalHeader;
    WORD Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD VirtualAddress;
    DWORD Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER {
    WORD Magic;
    BYTE MajorLinkerVersion;
    BYTE MinorLinkerVersion;
    DWORD SizeOfCode;
    DWORD SizeOfInitializedData;
    DWORD SizeOfUninitializedData;
    DWORD AddressOfEntryPoint;
    DWORD BaseOfCode;
    DWORD BaseOfData;
    DWORD ImageBase;
    DWORD SectionAlignment;
    DWORD FileAlignment;
    WORD MajorOperatingSystemVersion;
    WORD MinorOperatingSystemVersion;
    WORD MajorImageVersion;
    WORD MinorImageVersion;
    WORD MajorSubsystemVersion;
    WORD MinorSubsystemVersion;
    DWORD Win32VersionValue;
    DWORD SizeOfImage;
    DWORD SizeOfHeaders;
    DWORD CheckSum;
    WORD Subsystem;
    WORD DllCharacteristics;
    DWORD SizeOfStackReserve;
    DWORD SizeOfStackCommit;
    DWORD SizeOfHeapReserve;
    DWORD SizeOfHeapCommit;
    DWORD LoaderFlags;
    DWORD NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[16];
} IMAGE_OPTIONAL_HEADER, * PIMAGE_OPTIONAL_HEADER;

typedef struct _IMAGE_NT_HEADERS {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, * PIMAGE_NT_HEADERS;

typedef struct _IMAGE_EXPORT_DIRECTORY {
    DWORD Characteristics;
    DWORD TimeDateStamp;
    WORD MajorVersion;
    WORD MinorVersion;
    DWORD Name;
    DWORD Base;
    DWORD NumberOfFunctions;
    DWORD NumberOfNames;
    DWORD AddressOfFunctions;
    DWORD AddressOfNames;
    DWORD AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;

//下面全是函数
//不知道为什么 不加_就一直不给编译通过
void* malloc_(size_t size){
    return VirtualAlloc(NULL,size,0x1000,0x40);//0x40可读可写可访问权限
}

bool free_(void* address){
    return VirtualFree(address,0,0x8000);
}
void memzero(void* ptr, size_t size) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = 0;  // 将每个字节设为 0
    }
}
//取字符串长度
size_t strlen(const char *str) {
    const char* temp = str;
    size_t i = 0;
    while(*temp != '\0') {
        temp++;
        i++;
    }
    return i;
}
//k拷贝内存
void* memcpy(void* dest, const void* src, size_t n) {
    // 将 void 指针类型转换为 char 指针
    // 因为 char 类型的大小为 1 字节，可以按字节操作内存
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (unsigned char*)src;

    // 按字节复制
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest; // 返回目标地址
}

void intToStr(int num, char str[]) {
    int i = 0;
    int isNegative = 0;

    // 处理负数
    if (num < 0) {
        isNegative = 1;
        num = -num;  // 转换为正数处理
    }

    // 处理数字的每一位
    do {
        str[i++] = num % 10 + '0';  // 取最后一位数字并转换为字符
        num /= 10;  // 除以10以去掉最后一位
    } while (num != 0);

    // 如果是负数，则添加负号
    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';  // 添加字符串结束符

    // 由于数字是从低位到高位存储的，所以需要反转字符串
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void unsignedIntToStr(unsigned int num, char* str) {
    int i = 0;
    if (num == 0) {
        str[i++] = '0';  // 特殊情况：数字为0
    }
    else {
        int j;
        unsigned int temp = num;
        while (temp > 0) {
            temp /= 10;
            i++;
        }
        str[i] = '\0';  // 末尾添加字符串结束符
        while (num > 0) {
            str[--i] = (num % 10) + '0';  // 获取每一位的数字并转换为字符
            num /= 10;
        }
    }
}

void floatToStr(float num, char* str, int precision) {
    // 处理负数
    if (num < 0) {
        *str++ = '-';
        num = -num;  // 使数字变为正数
    }

    // 处理整数部分
    int intPart = (int)num;
    float fracPart = num - intPart;

    // 将整数部分转换为字符串
    char intStr[20];  // 临时存储整数部分
    int i = 0;
    if (intPart == 0) {
        intStr[i++] = '0';  // 特殊情况：整数部分为0
    }
    else {
        while (intPart > 0) {
            intStr[i++] = (intPart % 10) + '0';
            intPart /= 10;
        }
    }
    intStr[i] = '\0';

    // 将整数部分反转并拼接到目标字符串
    for (int j = i - 1; j >= 0; j--) {
        *str++ = intStr[j];
    }

    // 处理小数部分
    if (precision > 0) {
        *str++ = '.';  // 加小数点
        for (int i = 0; i < precision; i++) {
            fracPart *= 10;
            int fracInt = (int)fracPart;
            *str++ = fracInt + '0';  // 取小数部分的当前位并转换为字符
            fracPart -= fracInt;
        }
    }

    *str = '\0';  // 字符串结束
}
//十进制转16进制
void decimalToHexadecimal(int decimal, char* hexString) {
    char hexDigits[] = "0123456789ABCDEF";  // 十六进制的字符集
    int i = 0;

    // 处理特殊情况：当输入为0时，直接返回 "0"
    if (decimal == 0) {
        hexString[i++] = '0';
        hexString[i] = '\0';
        return;
    }

    // 十进制转十六进制
    while (decimal > 0) {
        int remainder = decimal % 16;  // 余数
        hexString[i++] = hexDigits[remainder];  // 存储对应的十六进制字符
        decimal /= 16;  // 更新商
    }

    hexString[i] = '\0';  // 字符串结束符

    // 反转字符串
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = hexString[start];
        hexString[start] = hexString[end];
        hexString[end] = temp;
        start++;
        end--;
    }

    // 确保字符串长度为8，如果不足，前面补0
    int len = strlen(hexString);
    if (len < 8) {
        // 移动现有字符，以便腾出空位
        for (int j = len; j >= 0; j--) {
            hexString[j + (8 - len)] = hexString[j];
        }
        // 填充0
        for (int j = 0; j < (8 - len); j++) {
            hexString[j] = '0';
        }
    }
}
//可变参数不能传入双精度浮点型

int strcmp(const char* str1, const char* str2) {
    // 遍历字符串，逐个字符比较
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    // 返回字符的差值
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}


bool callEPrintFunc(){
    DWORD hProcess = OpenProcess(0x1F0FFF,false,EPI->EProcessID);
    if(hProcess == 0){
        return false;
    }
    DWORD hThread = CreateRemoteThread(hProcess, NULL, 0, EPI->print_funcAdd, NULL, 0, NULL);
    if (hThread == 0) {
        CloseHandle(hProcess);
        return false;
    }
    WaitForSingleObject(hThread,0xFFFFFFFF);
    CloseHandle(hProcess);
     CloseHandle(hThread);
    return true;
}

void EPrintString(const char*format){
    size_t str_len = strlen(format);
    unsigned char* d = (unsigned char*)EPI->print_Buff;
    const unsigned char* s = (unsigned char*)format;

    // 按字节复制
    for (size_t i = 0; i < str_len; i++) {
        d[i] = s[i];
    }
    callEPrintFunc();
}
void __stdcall prin(const char*format,...){
    *(int*)EPI->print_Buff = 66;

    callEPrintFunc();
}

void __stdcall sprintf(char *str, const char *format, ...){
    //printf("长度为%d", strlen(format));
    size_t str_len = strlen(format)+1;
    //这里必须+2 因为ebp+4是返回地址  ebp+8是格式化的字符串参数 所以第二个参数是ebp+12
    int arg_num = 3;

    int index = 0;
    
    int arg_index = 0;

    //char *buff = (char*)malloc_(1024);//缓冲区最大为1024个字节 这是最后结果的缓存区
    char *buff = str;//缓冲区最大为1024个字节 这是最后结果的缓存区

    char buff_c[30] = {0};//字符的缓存区

    int b_int = 0;
    unsigned int b_uint = 0;
    float b_float = 0;
    double b_double = 0;
    //计算参数个数
    for (int i = 0; i < str_len; i++) {
        if (format[i] == '%') {
            i++;
            arg_num++;
            if (format[i] == 'd') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax,arg_index
                    add eax,ebp
                    mov eax,dword ptr[eax]
                    mov b_int,eax
                    pop eax
                }
                intToStr(b_int, buff_c);
                memcpy((char*)(&buff[index]), buff_c,strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c,30);
                continue;
            }
            else if (format[i] == 'u') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                unsignedIntToStr(b_uint, buff_c);
                memcpy((char*)(&buff[index]), buff_c, strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c, 30);
                continue;
            }
            else if (format[i] == 'c') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                buff[index++] = *(char*)(&b_uint) ;
                continue;
            }
            else if (format[i] == 's') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                memcpy((char*)(&buff[index]), (char*)b_uint, strlen((char*)b_uint));
                index += strlen((char*)b_uint);
                continue;
            }
            else if (format[i] == 'p') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                decimalToHexadecimal(b_uint, buff_c);
                memcpy((char*)(&buff[index]), buff_c, strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c, 30);
                continue;
            }
            
        }
        buff[index++] = format[i];
    }
}

//不知道什么情况 这个函数有幽灵bug 实在找不到问题
void __stdcall  Printf(const char*format,...) {
    
    //printf("长度为%d", strlen(format));
    size_t str_len = strlen(format)+1;
    //这里必须+2 因为ebp+4是返回地址  ebp+8是格式化的字符串参数 所以第二个参数是ebp+12
    int arg_num = 2;

    int index = 0;
    
    int arg_index = 0;

    //char *buff = (char*)malloc_(1024);//缓冲区最大为1024个字节 这是最后结果的缓存区
    char *buff = (char*)EPI->print_Buff;//缓冲区最大为1024个字节 这是最后结果的缓存区

    char buff_c[30] = {0};//字符的缓存区

    int b_int = 0;
    unsigned int b_uint = 0;
    float b_float = 0;
    double b_double = 0;
    //计算参数个数
    for (int i = 0; i < str_len; i++) {
        if (format[i] == '%') {
            i++;
            arg_num++;
            if (format[i] == 'd') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax,arg_index
                    add eax,ebp
                    mov eax,dword ptr[eax]
                    mov b_int,eax
                    pop eax
                }
                intToStr(b_int, buff_c);
                memcpy((char*)(&buff[index]), buff_c,strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c,30);
                continue;
            }
            else if (format[i] == 'u') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                unsignedIntToStr(b_uint, buff_c);
                memcpy((char*)(&buff[index]), buff_c, strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c, 30);
                continue;
            }
            else if (format[i] == 'c') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                buff[index++] = *(char*)(&b_uint) ;
                continue;
            }
            else if (format[i] == 's') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                memcpy((char*)(&buff[index]), (char*)b_uint, strlen((char*)b_uint));
                index += strlen((char*)b_uint);
                continue;
            }
            else if (format[i] == 'p') {
                arg_index = arg_num * 4;
                _asm {
                    push eax
                    mov eax, arg_index
                    add eax, ebp
                    mov eax, dword ptr[eax]
                    mov b_uint, eax
                    pop eax
                }
                decimalToHexadecimal(b_uint, buff_c);
                memcpy((char*)(&buff[index]), buff_c, strlen(buff_c));
                index += strlen(buff_c);
                memzero(buff_c, 30);
                continue;
            }
            
        }
        buff[index++] = format[i];
    }

   /* for (int i = 0; i < str_len; i++) {
        if (format[i] == '%') {
            i++;
            
        }
    }*/
    //*(int*)EPI->print_Buff = 10;
    //这里就直接调用易语言的输出函数吧
    //memcpy(EPI->print_Buff,buff,1024);
    callEPrintFunc();
   // 
    //free_(buff);
    //这时候怎么处理这个指针就看你了
    //printf("buff : %s", buff);
}

//这三个函数绝对不能修改
//_______________________________________________________________________

DWORD GetKernel32() {
   DWORD kernel32;
   /*  这是获取KERNELBASE的
    __asm {
        // 获取 PEB 结构体地址
        mov eax, fs: [0x30]              // 获取 PEB 地址
        mov eax, [eax + 0x0C]           // 获取 PEB_LDR_DATA 地址
        mov eax, [eax + 0x1C]           // 获取 InLoadOrderModuleList 链表头地址
        mov eax, [eax]                  // 获取链表中的第一个模块地址
        mov ebx, [eax + 0x8]            // 获取当前模块基地址
        mov kernel32, ebx               // 将基地址存入 kernel32
    }
    return kernel32;*/

   //这才是获取KERNEL32.DLL的
    __asm{
        push eax
        mov eax, fs: [0x30]
        mov eax, [eax + 0x0C]
        mov eax, [eax + 0x14]
        mov eax, [eax]
        mov eax, [eax]
        mov eax, [eax + 0x10]
        mov kernel32,eax
        pop eax
     }
    return kernel32;
}
// 自定义GetProcAddress函数
void* GetProcAddressCustom(DWORD hModule, const char* funcName) {
    // 获取DOS头
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (pDosHeader->e_magic != 0x5A4D) { // 检查MZ标志
        return NULL;
    }

    // 获取NT头
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != 0x00004550) { // 检查PE标志
        return NULL;
    }

    // 获取导出表RVA
    IMAGE_DATA_DIRECTORY exportDir = pNtHeaders->OptionalHeader.DataDirectory[0];
    if (exportDir.VirtualAddress == 0) {
        return NULL;
    }

    // 获取导出表
    PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)hModule + exportDir.VirtualAddress);

    // 获取导出函数名称表、序号表和地址表
    DWORD* pNames = (DWORD*)((BYTE*)hModule + pExportDir->AddressOfNames);
    WORD* pNameOrdinals = (WORD*)((BYTE*)hModule + pExportDir->AddressOfNameOrdinals);
    DWORD* pFunctions = (DWORD*)((BYTE*)hModule + pExportDir->AddressOfFunctions);

    // 遍历导出函数名称表
    for (DWORD i = 0; i < pExportDir->NumberOfNames; i++) {
        char* name = (char*)((BYTE*)hModule + pNames[i]);
        if (strcmp(name, funcName) == 0) {
            // 找到匹配的函数名称，返回函数地址
            DWORD functionRVA = pFunctions[pNameOrdinals[i]];
            return (void*)((BYTE*)hModule + functionRVA);
        }
    }

    return NULL;
}

void* GetfunctionAddress(const char* dllAddress, const char* funcName) {
    DWORD a = LoadLib(dllAddress);
    return GetproAdd(a, funcName);
}



//初始化全局变量表的函数地址 main函数第一句必须是调用这个
void initFuncAdd(){
    if(kernel32 == 1){
        kernel32 = GetKernel32();
    }
    if(GetproAdd == (GETPROADD)1){
        GetproAdd= (GETPROADD)GetProcAddressCustom(kernel32, "GetProcAddress");
    }
    if(LoadLib == (LOADLIB)1){
        LoadLib = (LOADLIB)GetProcAddressCustom(kernel32, "LoadLibraryA");
    }
    if(kernel32 == 0 || GetproAdd==0 || LoadLib == 0){
        return NULL;
    }
    VirtualAlloc = (VIRTUALALLOC)GetproAdd(kernel32, "VirtualAlloc");
    VirtualFree = (VIRTUALFREE)GetproAdd(kernel32, "VirtualFree");
    OpenProcess = (OPENPROCESS)GetproAdd(kernel32, "OpenProcess");
    CloseHandle = (CLOSEHANDLE)GetproAdd(kernel32, "CloseHandle");
    CreateRemoteThread = (CREATEREMOTETHREAD)GetproAdd(kernel32, "CreateRemoteThread");
    WaitForSingleObject = (WAITFORSINGLEOBJECT)GetproAdd(kernel32, "WaitForSingleObject");
}



//________________________________________________________________________