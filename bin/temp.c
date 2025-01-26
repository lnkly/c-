#include"std.h"


void main() {
    initFuncAdd();//必须调用这个初始化函数表
    /*__asm{
        pushad
        mov edi,0x231D43E0
        push 5//这是僵尸的坐标
        push 2
        mov eax,edi
        mov ebx,0x004236D0
        call ebx
        popad
    }

    EPrintString("创建了一只僵尸");
    //231D43E0
    char buff[1024]={0};
    int *p = (int*)0x231D9958;
    int sunNum = *p;
    *p = sunNum + 100;
    sprintf(buff,"阳光增加到了%d", *p);
    EPrintString(buff);*/
    //int* a  = (int*)EPI->print_Buff;
    //*a = 55;
    //callEPrintFunc();
    //prin("a");
   /*int a = 100;
    int b[5]={1,2,3,4,5};
    for(int i = 0;i<5;i++){
        Printf("数组b的第[%d]个元素为%d\n ",i,b[i]);
    }*/
    
 
    //callEPrintFunc();
    /*int a[5]={5,4,3,2,1};
    for (size_t i = 0; i < 5; i++)
    {
        printf("数组a的第[%d]个元素是：%d\n",i,a[i]);
    }
    */
   /*char buff[1024]={0};
   sprintf(buff,"这是一段文本 buff缓冲区的地址为%p",&buff);
   EPrintString(buff);*/

    /*char buff[1024]={0};
    int *a = (int*)malloc_(12);
    sprintf(buff,"申请的内存为%p",a);
    EPrintString(buff);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    for (size_t i = 0; i < 3; i++)
    {
        sprintf(buff,"数组a的第[%d]个元素是：%d\n",i,a[i]);
        EPrintString(buff);
    }
    free_(a);*/
    EPrintString("申请的空间释放成功");
    EPrintString("比如这个可以直接输出一段话");
    //int(__stdcall *messagebox)(int,char*,char*,int) = GetfunctionAddress("user32.dll","MessageBoxW");
    //messagebox(0,L"这是一条来自外部调用的消息",L"窗口标题",1);
    //int *sunAdd = (int*)0x22DFF9C8;
    //*sunAdd = 666;
    /*void(*AllocConsole)() =0x7749F1E0;
    AllocConsole();*/

    //DWORD k = GetKernel32();
    //DWORD G = GetProcAddressCustom(k, "GetProcAddress");
    return;
}