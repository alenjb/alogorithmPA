void tokenize(char token[]){

    char * funcTable[1000];
    char * varTable[1000];
    int funcTableSize = 0;
    int varTableSize = 0;
    int isAnnotation = 0;
    FILE *output_file = fopen("output.txt", "w");
    

    //주석이 아니고 #으로 시작하면 
    if (isAnnotation==0 && line[0] =='#'){

    }
    //주석이 아니고 #으로 시작 안하면

    while (line[i] != '\0') {
        if (line[i] == ' ') {
            line[i] = '\0';
            if(token == "my") printf("%s", token);
            int isPri =0; //기본 자료형이 나왔는지
            int isLparen =0; // ( 가 나왔는지
    //2. FUNC: 영어 + () + {
        char * nextToken = &line[i + 1];
        //포인터 있음 주의
        //이중 포인터 주의
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
                isPri =1;//기본 자료형이다.
                fprintf(output_file, "%s\n", token);
            }

        else if((strcmp(token, '*')==0) && (isPri ==1)){ //포인터
            fprintf(output_file, "%s\n", token);
        }
        else if(strcmp(token, "unsigned")==0){//unsigned
            fprintf(output_file, "%s ", token); //줄바꿈 안함
        }

            //함수가 처음 선언된 경우
        else if (isPri == 1 && token >='A' && token <= 'z' &&  *(nextToken-2) == ')' && *nextToken == '{') //전에 기본 자료형이 나왔고 첫 시작이 문자이고 마지막이 )로 끝나고 다음 토큰이 {이면
        {
            fprintf(output_file, "%s\n", "FUNC");
            funcTable[funcTableSize] = *token;
            funcTableSize++ ;
            isPri = 0;
        }else if (isPri ==1 && token >='A' && token <= 'z' )//VAR이 처음 선언된 경우
        {
            fprintf(output_file, "%s\n", "VAR");
            varTable[varTableSize] = *token;
            varTableSize++ ;
            isPri = 0;

        }else if (*token >= '0' && *token <= '9')//NUM
        {
            fprintf(output_file, "%s\n", "NUM_LITERAL");
            isPri = 0;
           
        }else if (*token >= '\'' || *token <= '\"')//String
        {
            fprintf(output_file, "%s\n", "String_LITERAL");
            isPri = 0;
            
        }else{//기타
            //함수 명에 있으면
            for (int i = 0; i < funcTableSize; i++) {
                if (funcTable[i] == *token) {
                    //함수 명에 있을 때
                    fprintf(output_file, "%s\n", "FUNC");
                    isPri = 0;
                    break;
                    }
                }

            //변수 명에 있으면
            for (int i = 0; i < varTableSize; i++) {
                if (varTable[i] == *token) {
                    //변수 명에 있을 때
                    fprintf(output_file, "%s\n", "VAR");
                    isPri = 0;
                    break;
                    }
                }
            //기타(1개 이상일 수 있음)
            //기본함수이면
            //기본 함수는 
            //1. ;로 잘라서 
            char * lastToken = token;
            while(*lastToken ==)
            //2. 공백으로 자르기
                //3-1, 개수가 2 이상이면 개수 기록하기
                //4-1. 이름이랑( 제거하고 이름출력 후 token은 ( 다음 부터 
                //5-1. , 제거하고 맞게 토크나이즈(개수만큼 반복)
                //6-1. );로 끝나면 ); 제거하고 마지막꺼 매치
                
                //3-2. 개수가 1개이면(인자가 하나)
                //4-2. 이름이랑( 제거하고 이름출력 후 token은 ( 다음 부터 
                //5-2. ); 제거하고 맞게 토크나이즈

            if (isPri == 0 && token >='A' && token <= 'z' &&  *(nextToken-2) == ')'){
                //( 전까지 출력
                //
                fprintf(output_file, "%s\n", token);
                isPri = 0;
                //작성 중
                break;
            }
        }
        

        
    //VAR: (기본 자료형 + 공백
        //정의될 때
        
        //이미 정의 되었을 때
    //NUM_LITERAL
        //'0~9' 로 시작하고 0~9로 끝나면
    //STR_LITERAL
        // "로 시작하고 "로 끝나면 또는 ' '
    //기타
        //그대로 출력

 
 
            token = &line[i + 1];
            }
        i++;
    }
    //마지막 토큰
    printf("%s", token);
    //token에 담긴 상태

    //printf("%s\n", token);
    // 변수 테이블
    //토큰화 하는 함수
    // for (int i=0; i<MAX_LINE_LENGTH; i++){
    //      if (line[i] == '\0') { // null 문자를 만나면 반복문 종료
    //         break;
    //1. 공백 단위로 자르기
        

    //2. FUNC: 영어 + () + {

    


    //VAR: (기본 자료형 + 공백
        //정의될 때
        
        //이미 정의 되었을 때
    //NUM_LITERAL
        //'0~9' 로 시작하고 0~9로 끝나면
    //STR_LITERAL
        // "로 시작하고 "로 끝나면 또는 ' '
    //기타
        //그대로 출력

    // }
    // "STR_LITERAL"
    // "NUM_LITERAL"
    // "VAR"
    // "FUNC"
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
    char line[MAX_LINE_LENGTH] ="hello my word";
    tokenize(line);
    return 0;
}