int a1 = 10;
int b1 = 20;
char str[20] = "hello world";
int add(int a,int b){
    return a+b;
}

int sub(int a ,int b){
    return a-b;
}
int main(void){
    int num = add(1,2);
    int d = sub(3,1);
int i;
    for( i= 0; i < num; i++){
        d=d+1;
    };
    return 0;
}