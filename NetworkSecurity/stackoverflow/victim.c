#include <stdio.h>
#include <string.h>

// 攻击者希望调用的恶意函数
void malicious_function()
{
    printf("!!! 恶意函数被成功调用 !!!\n");
    printf("*** 栈溢出攻击成功 ***\n");
}

// 存在漏洞的受害函数
void vulnerable_function()
{
    char buffer[64]; // 小缓冲区容易溢出

    printf("请输入一些文本: ");
    gets(buffer); // 不安全的函数，不检查输入长度

    printf("你输入的是: %s\n", buffer);
}

int main()
{
    vulnerable_function();
    printf("程序正常退出\n");
    return 0;
}