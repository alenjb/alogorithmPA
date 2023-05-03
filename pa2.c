#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000

//int static annotationStart = 0; // /*를 통해서 주석이 시작하면 1이됨
char funcTable[100][100];
char varTable[100][100];
int static funcTableSize = 0;
int static varTableSize = 0;
int static isPri =0;
int static isFirstFunc=0;
int static isFirstString =0;
int static wasNumorVar =0; //이전에 숫자나 변수가 나왔으면 1 아니면 0;
int static isAnnotation = -1; //-1이면 아직 확인 안한거, 0이면 확인했는데 아닌거, 1이면 주석인거
int static longAnnotation =0;

//long ann 시작하면 1 아니면 0
//isanno 존재하면 1 없으면 0 모르면 -1

void tokenize(char *token){

    printf("%s 를 토크나이즈 \n", token);
    FILE *output_file = fopen("output.txt", "a");
    //기본 자료형이면
    if(//포인턴 때매 strncmp로 함
        isPri ==0 &&(
        strncmp(token, "void" , 4) == 0 || 
        strncmp(token, "char" , 4) ==0 ||
        strncmp(token, "short", 5) ==0 ||
        strncmp(token, "int", 3) ==0 ||
        strncmp(token, "long", 4) ==0 ||
        strncmp(token, "float", 5) ==0 ||
        strncmp(token, "double", 6) ==0 ||
        strncmp(token, "enum", 4) ==0
        )){
            fprintf(output_file, "%s\n", token);
            isPri = 1;
            wasNumorVar =0;
            return;
        }
    //포인터 하나이면
    else if(strcmp(token, "*")==0){
        fprintf(output_file, "%s\n", token); //변수 출력
        wasNumorVar =0;
    return;
    }
    //포인터 변수이면
    else if(strncmp(token, "*", 1)==0){
        fprintf(output_file, "%s\n", "VAR"); //변수 출력
        wasNumorVar =1;
        isPri = 0;
        return;
    }
    // unsigned
    else if(strcmp(token, "unsigned")==0){ //unsigned
        fprintf(output_file, "%s ", token); //줄바꿈 안하고 출력
        wasNumorVar =0;
        return;
    }
    //static
    else if(strcmp(token, "static")==0){ //static
        fprintf(output_file, "%s\n", token); //그대로 출력
        wasNumorVar =0;
        return;
    }//num
    else if (*token >= '0' && *token <= '9'){ //NUM
        fprintf(output_file, "%s\n", "NUM_LITERAL"); //숫자라고 출력
        isPri = 0;
        wasNumorVar =1;
        return;
    }
    //String
    else if (*token == '\'' || *token == '\"'){ //String
        if(isFirstString ==0){ //처음 나온거면
            fprintf(output_file, "%s\n", "String_LITERAL");
            wasNumorVar =0;
            isPri = 0;
            isFirstString =1;   
            return;
        }
        else{//뒤에 나오는거면
            wasNumorVar =0;
            return;
        }
    }
    //함수 선언 OR 변수 선언
    else if(isPri ==1 && *token >='A' && *token <= 'z'){
        wasNumorVar =0;
        //함수 선언이면
        if(isFirstFunc ==1 ){
            fprintf(output_file, "%s\n", "FUNC");
            int len = strlen(token);
            strcpy(funcTable[funcTableSize], token);
            printf(" %d번쨰에 함수에 추가:  %s \n",funcTableSize, token );
            funcTableSize++;
            isPri = 0;
            isFirstFunc =0;
            return;
        }
        //변수 선언이면
        else{
            fprintf(output_file, "%s\n", "VAR");
            strcpy(varTable[varTableSize], token);
            printf("%d번쨰에  변수에 추가:  %s \n",varTableSize, token );

            varTableSize++;
            wasNumorVar=1;
            isPri = 0;
            return;
        }
    }
    //이미 선언된 함수 OR 변수 이면
    else if(isPri ==0 && *token >='A' && *token <= 'z'){
        wasNumorVar =0;
        // 이미 선언된 함수 이면
        for (int i = 0; i <funcTableSize; i++) {
            if (strcmp(funcTable[i], token) == 0) {
                //함수 명에 있을 때
                fprintf(output_file, "%s\n", "FUNC");
                isPri = 0;
                return;
            }
        }

        // 이미 선언된 변수 이면
        for (int i = 0; i < varTableSize; i++) {
            if (strcmp(varTable[i], token) == 0) {
                //변수 명에 있을 때
                fprintf(output_file, "%s\n", "VAR");
                wasNumorVar =1;
                isPri = 0;
                return;
            }
        }
        //개행문자나 특수문자이면
        if(*token == '\\' || *token == '%'){
            wasNumorVar =0;
            return;
        }
        //기타
        fprintf(output_file, "%s\n", token);
        wasNumorVar =0;
        return;
    }
    //기타 # 포함

        // {( [ ]) }이면
    else if(
        *token =='{'||
        *token =='}'||
        *token ==')'||
        *token ==')'||
        *token =='['||
        *token ==']'
    ){
        wasNumorVar =0;
        return;
    }
    //산술 연산자, 증감연산자, +=, -=, *-, /=이  껴있으면 (%따로 다루기)
    else if (*token == '+' || *token == '-' || *token == '*' || *token == '/') {
        int tokenLen = strlen(token);
        

        if(tokenLen ==1){ //연산자만 들어오면
            if(wasNumorVar ==1){// 전에 숫자나 문자가 나온 경우 연산자 이므로
                fprintf(output_file, "%s\n", token );
                wasNumorVar =0;
            }
            else if (wasNumorVar ==0 && ((*token == '+')|| (*token == '-'))){ //음수나 양수를 표현하기 위한 것
                fprintf(output_file, "%s", token );
                wasNumorVar =0;
            }
            
            return;
        }
        else{ //연산자랑 무언가가 붙어있으면
            // +이면
            if(*token =='+'){
                if(*(token + 1) == '+'){//++일 때
                fprintf(output_file, "++\n" );
                memmove(token, token+2, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
                }
                else if(*(token + 1) == '='){//+=일 때
                fprintf(output_file, "+=\n" );
                memmove(token, token+2, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
                }
                //+랑 뭔가가 붙어있으면 55 +34 , +34
                else{
                    if(wasNumorVar ==1){// 전에 숫자나 문자가 나온 경우 연산자 이므로
                    fprintf(output_file, "+\n");
                    wasNumorVar =0;
                    memmove(token, token+1, tokenLen);
                    tokenize(token);
                    return;
                    }
                    else if (wasNumorVar ==0 && *token == '+'){ //양수를 표현하기 위한 것
                        fprintf(output_file, "+");
                        wasNumorVar =0;
                        memmove(token, token+1, tokenLen);
                        tokenize(token);
                        return;
                    }
                }
            }
            //-이면
            else if(*token =='-'){
                
                if(*(token + 1) == '-'){//--일 때
                fprintf(output_file, "--\n" );
                memmove(token, token+2, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
                }
                else if(*(token + 1) == '='){//-=일 때
                fprintf(output_file, "-=\n" );
                memmove(token, token+2, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
                }
                //-랑 뭔가가 붙어있으면
                else{
                    if(wasNumorVar ==1){// 전에 숫자나 문자가 나온 경우 연산자 이므로
                        fprintf(output_file, "-\n");
                        wasNumorVar =0;
                        memmove(token, token+1, tokenLen);
                        tokenize(token);
                        return;
                    }
                    else if (wasNumorVar ==0 && *token == '-'){ //음수를 표현하기 위한 것
                        fprintf(output_file, "-");
                        wasNumorVar =0;
                        memmove(token, token+1, tokenLen);
                        tokenize(token);
                        return;
                    }                    
                }
            }
            // *=이면
            else if(*token =='*' && *(token + 1) == '='){
                fprintf(output_file, "*=\n" );
                memmove(token, token+2, tokenLen);
                wasNumorVar =0;
                }
            // /=이면
            else if(*token =='/' && *(token + 1) == '='){
                fprintf(output_file, "/=\n" );
                memmove(token, token+2, tokenLen);
                wasNumorVar =0;
                }
            //*와 붙어있으면
            else if(*token =='*'){
                fprintf(output_file, "*\n" );
                memmove(token, token+1, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
            }
            // /와 붙어있으면
            else if(*token =='/'){
                fprintf(output_file, "/\n" );
                memmove(token, token+1, tokenLen);
                tokenize(token);
                wasNumorVar =0;
                return;
            }
            
            return;

        }


    }
    //대입 연산자가 껴있으면
    else if (*token == '=') {
        int tokenLen = strlen(token);
        wasNumorVar =0;
        if(tokenLen ==1){ //= 하나이면
            fprintf(output_file, "=\n");
            return;
        }else{//=뒤에 뭔가 붙어있으면
           fprintf(output_file, "=\n" );
            memmove(token, token+1, tokenLen);
            tokenize(token);
            return; 
        }
    }


    else{
        fprintf(output_file, "%s\n", token);
        printf("%s 를 적음\n", token);
        wasNumorVar =0;
        return;
    }
}

void lineControl(char line[], int isAnnotation){
    printf("%s 를 라인 컨트롤  %d\n",line, isAnnotation);
    char *token = line;
    int lineLength=0;
    int semicolon =0;
    int comma =0;

    if(isAnnotation == 0 || isAnnotation == -1){ //만약 주석이 아니거나 존재하는지 모르면 short와 long은 0으로 초기화
        longAnnotation =0;
    }
    
    //한 줄의 길이 찾기 및 세미콜론, 콤마 찾기
    for(int i=0; i<MAX_LINE_LENGTH; i++){
        if(line[i] == '\n'){
            line[i]=' '; //줄바꿈 제거
            break;
        }
        else if(line[i] == ';'){
            semicolon++;
            line[i] = ' '; 
        }
        else if(line[i] == ','){
            comma ++;
        }
        else if(line[i] ==')' || line[i] =='('){
            line[i] = ' ';
        }
    }
    lineLength = strlen(line)-1;
    printf("linelen: %d\n", lineLength);
    //주석인지 확인했는데 주석이면
    //긴 주석만 하면 됨(짧은 주석은 memmove으로 처리)
    if(longAnnotation ==1){ // /*를 통해서 주석이 시작되었으면
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '*' && line[i+1] == '/'){ // */를 찾으면
                 memmove(line, line+i+2, lineLength-i-1);//그 뒤부터 시작되게 line을 변경
                 lineControl(line, -1); //바뀐 라인을 다시 컨트롤
                 return;
            }
        }
        if (longAnnotation == 1) return; //아직 주석 끝이 안나왔으면
    }

    //아직 주석인지 확인을 안했으면
    else if(isAnnotation == -1){
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '/'){
                if(line[0] == '/' && line[1] == '/'){ // //로 시작하는 주석만있는 줄이면 
                    printf("여ㅑ기 ㅇ말ㄴㄹ");
                    return;
                }
                else if(line[i+1] == '/'){// //가 있으면
                line[i] = '\0'; // //전까지만 가리키도록 함
                lineControl(line, 0); //바뀐 라인을 다시 컨트롤
                return;
                }
                else if (line[i+1] == '*'){ // /*가 있으면
                    isAnnotation = 1;
                    line[i] = '\0'; // /*전까지만 가리키도록 함
                    longAnnotation = 1; //긴 주석 시작
                    lineControl(line, 1); //바뀐 라인을 다시 컨트롤
                    return;
                }
            } 
        }
        //주석이 없고 다시 라인 컨트롤
        lineControl(line, 0);
    }
    //주석확인을 했는데 주석이 아니면
    else{
        //;가 있으면 ;로 자르기
        if(semicolon >0){
            printf("; 있음\n");
            //공백으로 자르기
            token = strtok(line, " ");
            while (token != NULL) {
                //토큰화 하기
                tokenize(token);
                //다음 토큰 만들기
                token = strtok(NULL, " ");
            }
            return;

        }
        // ;가 없으면
        else{
            printf(";없음\n");
            int startPri =0; //기본형으로 시작하는지 여부 0이면 아니고 1이면 맞음

            if(line[0] == '#'){// #include나 #define의 경우
                int f;
                //공백이 아닌 첫 문자를 찾기
                for(f=1; f<lineLength; f++){
                    if(line[f]!=' ') break;
                }
                if(line[f] == 'i' ){// #include
                    tokenize("#include");
                    memmove(line, line+f+7, lineLength-1);//그 뒤부터 시작되게 line을 변경
                    lineControl(line, -1);
                }
                else if(line[f] == 'd'){
                    tokenize("#define");
                    memmove(line, line+f+6, lineLength-1);//그 뒤부터 시작되게 line을 변경
                    lineControl(line, -1);
 
                }
                return;
            }

            // }하나만으로 이루어져있는지 확인
            else if(lineLength==1 && line[0] == '}'){
                return;
            }

            // {로 끝나는지 확인
            else if(line[lineLength-1] == '{'){
                printf("{ 로 끝s\n");
                line[lineLength-1] == ' ';
                //startPri 검증
                if(//포인턴 때매 strncmp로 함
                    strncmp(token, "void" , 4) == 0 || 
                    strncmp(token, "char" , 4) ==0 ||
                    strncmp(token, "short", 5) ==0 ||
                    strncmp(token, "int", 3) ==0 ||
                    strncmp(token, "long", 4) ==0 ||
                    strncmp(token, "float", 5) ==0 ||
                    strncmp(token, "double", 6) ==0 ||
                    strncmp(token, "enum", 4) ==0
                    ){
                    startPri =1;//기본 자료형이다.
//                    tokenize(token);
                    //기본 자료형을 출력                    
         //           fprintf(output_file, "%s\n", token);
                }
                //startPri가 1인지 확인 : 함수의 선언인지 확인
                if(startPri == 1){
                    isFirstFunc =1;
                    //콤마가 없으면 : 인자가 없는 함수 선언
                    if(comma ==0){
                        //공백으로 나누기
                        token = strtok(line, " ");
                        while (token != NULL) {
                            //토큰화 하기
                            tokenize(token);
                            //다음 토큰 만들기
                            token = strtok(NULL, " ");
                        }
                        return;
                    }
                    //콤마가 있으면 : 인자가 있는 함수 선언
                    else if(comma >0){
                        //공백으로 나누기
                        token = strtok(line, " ");
                        while (token != NULL) {
                            //토큰화 하기
                            tokenize(token);
                            //다음 토큰 만들기
                            token = strtok(NULL, " ");
                        }
                        return;
                    }
                }
                //isPri가 0인지 확인 : 제어문인지 확인
                else if (startPri == 0){
                    //일단 넘어감
                }
            }
            //기타 #포함
            else{
                printf("충격화 공고 %d\n", lineLength);
                printf("line[lineLength-1]:%c\n",line[lineLength]);
                //공백으로 나누기
                token = strtok(line, " ");
                while (token != NULL) {
                    //토큰화 하기
                    tokenize(token);
                    //다음 토큰 만들기
                    token = strtok(NULL, " ");
                }
                return;
            }
        }
    }
}


int main(int argc, char *argv[]) {

/*
    char *filename1 = argv[1];
    char *filename2 = argv[2];

    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");

    char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file1) != NULL) {
         
            printf("%s", line);
        }

    fclose(file1);
    fclose(file2);
*/
    FILE *file1 = fopen("input.txt", "r");
    char line[MAX_LINE_LENGTH];
    FILE *output_file = fopen("output.txt", "w");
    fprintf(output_file, "%s", "");

    while (fgets(line, MAX_LINE_LENGTH, file1) != NULL) {
        lineControl(line, -1);    
    }
    printf("---------------\n");
    for(int i=0; i<funcTableSize; i++){
        printf(" %d 일 때%s\n",i, funcTable[i]);
    }
    printf("---------------\n");
    for(int i=0; i<varTableSize; i++){
        printf("%d일 때 %s\n",i,varTable[i]);
    }
    return 0;
}