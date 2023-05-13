#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
# define MAX_LINE_LENGTH 1000
/*
포인터가 따로 출력된다고 가정한 ver

*/
//int static annotationStart = 0; // /*를 통해서 주석이 시작하면 1이됨
char funcTable[100][100];
char varTable[100][100];
int static funcTableSize = 0;
int static varTableSize = 0;
int static isPri =0; //전에 기본 자료형이 나왔는지
int static isFirstFunc=0; // 이 줄에 함수가 있는지를 line control에서 판단
int static isFirstString =0; //토크나이즈에서 판별할 때 문자열이 시작되었으면
// 더 먼저 시작한게 더 높은 숫자(isFirstStringOne과 isFirstStringTwo의 값중 먼저 시작한게 더 높은 값임)
int static isFirstStringOne =0; //' 로 시작했으면
int static isFirstStringTwo =0; // "로 시작했으면
int static isAnnotation = -1; //주석인지 확인: -1이면 아직 확인 안한거, 0이면 확인했는데 아닌거, 1이면 주석인거
int static isLongAnnotation =0;// "/*"로 시작하는 주석인지 확인: 0이면 아닌거, 1이면 주석인거
int static wassharp =0;// #만 나왔으면 -1, #define같이 같이 나오면 1 아니면 0
int static isDefineLine =0;// define이 나오는 줄을 처리할 때 1 아니면 0
int static defineFunc =0;// define이 나오는 줄에 함수가 안나오면 0 나왔는데 처리 안되었으면 -1 처리되었으면 1
int static printStar =0; //1이면 별 출력 0이면 아님 10이면 lcs 출력 11이면 표절률 출력
int static writingFileNo =0; //1이면 1번 파일 처리 중, 2이면 2번 파일 처리중

char** tokens1 = NULL;
int len1 = 0;
char** tokens2 = NULL;
int len2 = 0;
//long ann 시작하면 1 아니면 0
//isanno 존재하면 1 없으면 0 모르면 -1


void add_token(char*** tokens, const char* token) {
    if(writingFileNo ==1) {
        len1++;
        *tokens = (char**)realloc(*tokens, len1 * sizeof(char*));
        (*tokens)[len1 - 1] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        strcpy((*tokens)[len1 - 1], token);

    }
    if(writingFileNo==2){
        len2++;
        *tokens = (char**)realloc(*tokens, len2 * sizeof(char*));
        (*tokens)[len2 - 1] = (char*)malloc((strlen(token) + 1) * sizeof(char));
        strcpy((*tokens)[len2 - 1], token);

    }
}
char** find_longest_common_subsequence(char** tokens1, int len1, char** tokens2, int len2, int* lcs_len) {
    int m = len1;
    int n = len2;

    // 2차원 배열을 생성하여 공통 부분 수열의 길이를 저장
    int** dp = (int**)malloc((m + 1) * sizeof(int*));
    for (int i = 0; i <= m; i++) {
        dp[i] = (int*)malloc((n + 1) * sizeof(int));
        memset(dp[i], 0, (n + 1) * sizeof(int));
    }

    // 최장 공통 부분 수열의 길이 계산
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (strcmp(tokens1[i - 1], tokens2[j - 1]) == 0) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else {
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1]) ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }

    // 최장 공통 부분 수열 추적
    *lcs_len = dp[m][n];
    char** lcs = (char**)malloc(*lcs_len * sizeof(char*));
    int idx = *lcs_len - 1;
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (strcmp(tokens1[i - 1], tokens2[j - 1]) == 0) {
            lcs[idx] = tokens1[i - 1];
            idx--;
            i--;
            j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        }
        else {
            j--;
        }
    }

    // 메모리 해제
    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }
    free(dp);

    return lcs;
}


void tokenize(char *token){
    
    FILE *output_file = fopen("hw2_output.txt", "a");
    if(printStar==1){
        fprintf(output_file, "%s\n", "*****");
        printStar =0;
        return;
    }
    if(printStar ==10 || printStar ==11){ //lcs 출력 or 표절률 출력
        int lcs_len;
        char** lcs = find_longest_common_subsequence(tokens1, len1, tokens2, len2, &lcs_len);
        if(printStar ==10){//lcs 출력
            for (int i = 0; i < lcs_len; i++) {
                fprintf(output_file, "%s\n",lcs[i]);    
            }
        }
        else if(printStar==11){ //표절률 출력
            int shortLen = 0;
            if(len1 > len2){ //len2가 더 짧은 경우
                shortLen = len2;
            }else shortLen = len1;
            double result =  (double)lcs_len /shortLen *100 ;
            result = round(result * 100 )/ 100;
            if (!isnan(result)) {
                fprintf(output_file, "%.2f\n", result);
            } else {
                fprintf(output_file, "00.00\n");
            }
        }
        return;
    }
    // return;
    //공백이면 리턴하기
    if(strlen(token)==0) {
        return;
    }
    //공백제거
    int nonspace=0;
    //탭 제거
    while(token[nonspace]=='\t'){
        nonspace = nonspace + 1;
    }
    if(nonspace!=0) {
        tokenize(token+nonspace);
        
        return;
        };
    //공백 제거
    while(token[nonspace]==' '){
        nonspace = nonspace + 1;
    }
    if(nonspace!=0) {
        tokenize(token+nonspace);
        return;
        };
    
    //문자열 안에 있는 거면
    if(isFirstString == 1 ){
        if(isFirstStringOne>=1 && isFirstStringTwo>=1){ //둘 다 나왔고 중복해서 나왔으면
            if(isFirstStringOne > isFirstStringTwo){ // ' " " ' 이런 경우라면 
                for(int i=0; i<strlen(token); i++){
                    if(token[i] =='\"'){//"를 만나면
                        memmove(token, token+i+1, strlen(token));
                        isFirstStringTwo =0;
                        tokenize(token);
                        return;
                    }
                    if(token[i] =='\''){//'를 만나면
                        memmove(token, token+i+1, strlen(token));
                        isFirstStringOne =0;
                        tokenize(token);
                        return; 
                    }
                }   
            }
            else if(isFirstStringOne < isFirstStringTwo){ // " ' ' " 이런 경우라면 
                for(int i=0; i<strlen(token); i++){
                    if(token[i] =='\''){//'를 만나면
                        memmove(token, token+i+1, strlen(token));
                        isFirstStringOne =0;
                        tokenize(token);
                        return; 
                    }
                    if(token[i] =='\"'){//"를 만나면
                        memmove(token, token+i+1, strlen(token));
                        isFirstStringTwo =0;
                        tokenize(token);
                        return;
                    }
                }   
            }
        }
        else if(isFirstStringOne==1){ // '로 시작한 경우
            for(int i=0; i<strlen(token); i++){
                if(token[i] =='\''){//'를 만나면
                    memmove(token, token+i+1, strlen(token));
                    isFirstStringOne--;
                    if(isFirstStringOne + isFirstStringTwo==0){//문자열이 끝났으면
                        isFirstString =0;
                    }
                    tokenize(token);                    
                    return; 
                }

            }
        }
        else if(isFirstStringTwo==1){ // "로 시작한 경우
            for(int i=0; i<strlen(token); i++){
                if(token[i] =='\"'){//"를 만나면
                    memmove(token, token+i+1, strlen(token));
                    isFirstStringTwo--;
                    if(isFirstStringOne + isFirstStringTwo==0){//문자열이 끝났으면
                        isFirstString =0;
                    }

                    tokenize(token);
                    return; 
                }
            }
        }
        //시작은 했지만 둘 다 만나지 못하면 무시해야하므로
        return;
    }

    //기본 자료형이면
    if(
        strcmp(token, "void") == 0 || 
        strcmp(token, "char") == 0 ||
        strcmp(token, "short") == 0 ||
        strcmp(token, "int") == 0 ||
        strcmp(token, "long") == 0 ||
        strcmp(token, "float") == 0 ||
        strcmp(token, "double") == 0 ||
        strcmp(token, "enum") == 0 ||
        strcmp(token, "FILE") == 0 ||
        strcmp(token, "struct") == 0 ||
        strcmp(token, "typedef") == 0 ||
        strcmp(token, "union") == 0
        ){
            if(writingFileNo ==1){
                add_token(&tokens1, token);
            }else if(writingFileNo==2){
                add_token(&tokens2, token);
            }
            fprintf(output_file, "%s\n", token);
            isPri = 1;
            return;
        }
    //콤마가 있는 경우
    char *comma = strchr(token, ',');
    int commaidx = -1;
    if(comma!= NULL ){ //,가 존자
        int commaidx = comma - token; // ,의 인덱스 계산
        //콤마 앞에 뭐가 있으면
        if(commaidx>0){
            char* substring = (char*)malloc(commaidx+1);
            strncpy(substring, token, commaidx);
            substring[commaidx] = '\0';
            tokenize(substring);
        }
        //콤마 뒤에 뭐가 있으면
        if(commaidx+1 < strlen(token)){
            char* substring = (char*)malloc(strlen(token)- commaidx + 1);
            strcpy(substring, token + commaidx +1);
            tokenize(substring);
        } 
        return;
    }

    //<stdio.h>같은 경우이면
    if(token[0] == '<' && token[strlen(token)-1] == '>'){
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }
        fprintf(output_file, "%s\n", token);
        return;
    }

    //단일 연산자이면
    if(strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
        strcmp(token, "%") == 0 || strcmp(token, "=") == 0 || strcmp(token, ">") == 0 || strcmp(token, "<") == 0 ||
        strcmp(token, "!") == 0 || strcmp(token, "&") == 0 || strcmp(token, "|") == 0 || strcmp(token, "^") == 0 ||
        strcmp(token, "~") == 0){
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }    
        fprintf(output_file, "%s\n", token);
        return;
    }
    // ->로 시작하면
    if(strncmp(token, "->", 2) == 0){
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }    
        fprintf(output_file, "%s\n", token);
        // 뒤에 더 있으면
        if(strlen(token)!=2){
            int start =2;
            //공백이 아닌 부분을 찾기
            for(int i=1; i<strlen(token); i++){
                if(token[i] != ' '){
                    start = i;
                    break;
                }
            }
            char* substring = (char*)malloc(strlen(token)- start + 1);
            strcpy(substring, token + start);
            substring[strlen(token)- start] = '\0';
            tokenize(substring);
            }
            return;
    }

    //연산자가 포함되어 있으면
    int opidx = -1; //토큰에서 연산자의 위치
    //연산자의 위치를 찾기
    for (int i = 0; i < strlen(token); i++) {
        char* specialChar = strchr("+-*%/=><!&|^~", token[i]);
        if(specialChar != NULL) {
            opidx = i; 
            break;
        }
    }
    if(opidx!= -1){ //연산자가 존재하면
        if(opidx!=0){ //연산자 전에 무언가가 있으면
            char* substring = (char*)malloc(opidx+1);
            strncpy(substring, token, opidx);
            substring[opidx] = '\0';
            tokenize(substring);
        }
        char * op = (char*)malloc(2);
        strncpy(op, &token[opidx], 1);
        op[1] = '\0';
        tokenize(op);
        // fprintf(output_file, "%c\n", token[opidx]);//연산자 출력
        if(strlen(token)> opidx+1){//연산자 뒤에 무언가가 있으면
            char* substring = (char*)malloc(strlen(token)- opidx + 1);
            strcpy(substring, token + opidx +1);
            tokenize(substring);
        }
        
        return;
    }
    


    // unsigned
    else if(strcmp(token, "unsigned")==0){ //unsigned
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }
        fprintf(output_file, "%s ", token); //줄바꿈 안하고 출력
        return;
    }
    //static
    else if(strcmp(token, "static")==0){ //static
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }
        fprintf(output_file, "%s\n", token); //그대로 출력
        return;
    }//num
    else if (*token >= '0' && *token <= '9'){ //NUM
        if(writingFileNo ==1){
            add_token(&tokens1, "NUM_LITERAL");
        }else if(writingFileNo==2){
            add_token(&tokens2, "NUM_LITERAL");
        }
        fprintf(output_file, "%s\n", "NUM_LITERAL"); //숫자라고 출력
        isPri = 0;
        return;
    }
    //String ''
    else if (*token == '\'' ){ // ''
        if(isFirstString ==0 &&isFirstStringOne ==0){ //처음 나온거면
            //끝나는 '가 있는지 탐색
            int stridx=-1;
            for (int i = 1; i < strlen(token); i++) {
                char* endstr = strchr("\'", token[i]);
                if(endstr != NULL) { //끝나는 '가 있으면
                    stridx =i;    
                    break;
                }
            }
            if(stridx!=-1){//끝나는 '가 있으면
                if(writingFileNo ==1){
                    add_token(&tokens1, "STR_LITERAL");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "STR_LITERAL");
                }
                fprintf(output_file, "%s\n", "STR_LITERAL");
                isPri = 0;
                isFirstStringOne =0;
                if(stridx!=strlen(token)-1){ //뒤에 뭐가 더 있으면
                    char* substring = (char*)malloc(strlen(token)- stridx + 1);
                    strcpy(substring, token + stridx +1);
                    tokenize(substring);
                    return;
                }
                return;
            }
            //끝나는 '가 없으면
            if(writingFileNo ==1){
                add_token(&tokens1, "STR_LITERAL");
            }else if(writingFileNo==2){
                add_token(&tokens2, "STR_LITERAL");
            }
            fprintf(output_file, "%s\n", "STR_LITERAL");
            isPri = 0;
            isFirstString =1;
            isFirstStringOne =1;
            if(isFirstStringTwo>0) isFirstStringTwo++;   
            return;
        }
    }
        //String ""
    else if (*token == '\"'){ //String
        if(isFirstString ==0 && isFirstStringTwo ==0){ //처음 나온거면
            //끝나는 "가 있는지 탐색
            int stridx=-1;
            for (int i = 1; i < strlen(token); i++) {
                char* endstr = strchr("\"", token[i]);
                if(endstr != NULL) { //끝나는 "가 있으면
                    stridx =i;    
                    break;
                }
            }
            if(stridx!=-1){//끝나는 "가 있으면
                if(writingFileNo ==1){
                    add_token(&tokens1, "STR_LITERAL");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "STR_LITERAL");
                }
                fprintf(output_file, "%s\n", "STR_LITERAL");
                isPri = 0;
                isFirstStringTwo =0;
                if(stridx!=strlen(token)-1){ //뒤에 뭐가 더 있으면
                    char* substring = (char*)malloc(strlen(token)- stridx + 1);
                    strcpy(substring, token + stridx +1);
                    tokenize(substring);
                    return;
                }
                // "로 끝나면
                return;
            }
            //끝나는 '가 없으면
            if(writingFileNo ==1){
                add_token(&tokens1, "STR_LITERAL");
            }else if(writingFileNo==2){
                add_token(&tokens2, "STR_LITERAL");
            }
            fprintf(output_file, "%s\n", "STR_LITERAL");
            isPri = 0;
            isFirstString =1;
            isFirstStringTwo =1;
            if(isFirstStringOne>0) isFirstStringOne++;   
            return;
        }
        
    }


    //배열의 경우
    char *leftArr = strchr(token, '[');
    char *rightArr = strchr(token, ']');
    if(leftArr!= NULL && rightArr != NULL){ //[과 ]이 존재하는 경우 = 배열인 경우
    
        int leftIndex = leftArr - token; // '['의 인덱스 계산
        int rightIndex = rightArr - token; // ']'의 인덱스 계산
        //[의 인덱스가 처음이 아니면: arr[5], arr[5]=
        if(leftIndex!=0){
            //앞부분
            char* frontArr = (char*)malloc(leftIndex+1);
            strncpy(frontArr, token, leftIndex);
            frontArr[leftIndex] = '\0';
            //뒷부분
            char* substring = (char*)malloc(strlen(token)- leftIndex + 1);
            strcpy(substring, token + leftIndex);
            
            substring[strlen(token)- leftIndex] = '\0';
            //앞의 부분 토크나이즈
            tokenize(frontArr);
            //[부터 다시 토크나이즈
            tokenize(substring);
            return;
        }else{ //else:([가 첫번째 인덱스이면)    [5], [5]=5

            isPri =0;
            // 뒤의 인덱스가 있으면:[5]=5
            if(rightIndex!= strlen(token)-1){
                char* substring = (char*)malloc(strlen(token)- rightIndex + 1);
                strcpy(substring, token + rightIndex +1);
                substring[strlen(token)- rightIndex] = '\0';

                // ]뒤의 인덱스부터 끝까지를 토크나이즈
                tokenize(substring);
            }
            // [다음 인덱스부터 ]전까지 인덱스를 토크나이즈
        
            char* frontstring = (char*)malloc(rightIndex);
            strncpy(frontstring, token + leftIndex+1, rightIndex- leftIndex-1);
            frontstring[rightIndex- leftIndex-1] = '\0';
            tokenize(frontstring);
            return;        
        }
    }

    //함수 선언 OR 변수 선언
    else if(isPri ==1 && *token >='A' && *token <= 'z'){
        //함수 선언이면
        if(isFirstFunc ==1 ){
            if(writingFileNo ==1){
                add_token(&tokens1, "FUNC");
            }else if(writingFileNo==2){
                add_token(&tokens2, "FUNC");
            }
            fprintf(output_file, "%s\n", "FUNC");
            int len = strlen(token);
            strcpy(funcTable[funcTableSize], token);
            funcTableSize++;
            isPri = 0;
            isFirstFunc =0;
            return;
        }
        //변수 선언이면
        else{
            if(writingFileNo ==1){
                add_token(&tokens1, "VAR");
            }else if(writingFileNo==2){
                add_token(&tokens2, "VAR");
            }
            fprintf(output_file, "%s\n", "VAR");
            strcpy(varTable[varTableSize], token);
            varTableSize++;
            isPri = 0;
            return;
        }
    }
    //이미 선언된 함수 OR 변수 이면
    else if(isPri ==0 && *token >='A' && *token <= 'z'){
        // 이미 선언된 함수 이면
        for (int i = 0; i <funcTableSize; i++) {
            if (strcmp(funcTable[i], token) == 0) {
                //함수 명에 있을 때
                if(writingFileNo ==1){
                    add_token(&tokens1, "FUNC");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "FUNC");
                }
                fprintf(output_file, "%s\n", "FUNC");
                isPri = 0;
                if(defineFunc==-1) defineFunc =1;
                return;
            }
        }

        // 이미 선언된 변수 이면
        for (int i = 0; i < varTableSize; i++) {
            if (strcmp(varTable[i], token) == 0) {
                //변수 명에 있을 때
                if(writingFileNo ==1){
                    add_token(&tokens1, "VAR");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "VAR");
                }
                fprintf(output_file, "%s\n", "VAR");
                isPri = 0;
                return;
            }
        }

        //개행문자나 특수문자이면
        if(*token == '\\' || *token == '%'){
            return;
        }
        //# 다음에 나온거면
        if(wassharp ==-1){
            if(writingFileNo ==1){
                add_token(&tokens1, token);
            }else if(writingFileNo==2){
                add_token(&tokens2, token);
            }
            fprintf(output_file, "%s\n", token);
            wassharp =1;
            return;            
        }
        //#define 다음에 나온거면
        if(wassharp ==1 || isDefineLine == 1){
            //define이 나오는 줄에 함수가 없는 경우
            if(isDefineLine==1 && defineFunc == 0){
                if(writingFileNo ==1){
                    add_token(&tokens1, "VAR");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "VAR");
                }
                fprintf(output_file, "%s\n", "VAR");
                strcpy(varTable[varTableSize], token);
                varTableSize++;
                wassharp =0;
                return;
            }
            //define이 나오는 줄에 함수가 있는데 아직 처리가 안된 경우
            else if(isDefineLine==1 && defineFunc == -1){
                if(writingFileNo ==1){
                    add_token(&tokens1, "FUNC");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "FUNC");
                }
                fprintf(output_file, "%s\n", "FUNC");
                strcpy(funcTable[funcTableSize], token);
                funcTableSize++;
                wassharp =0;
                defineFunc = 1;
                return;
            }
            //define이 나오는 줄에 함수가 있고 처리된 경우
            else if(isDefineLine==1 && defineFunc == 1){
                if(writingFileNo ==1){
                    add_token(&tokens1, "VAR");
                }else if(writingFileNo==2){
                    add_token(&tokens2, "VAR");
                }
                fprintf(output_file, "%s\n", "VAR");
                wassharp =0;
                return;            
            }
        }
        //<stdio.h>같은 경우이면
        if(token[0] == '<' && token[strlen(token)-1] == '>'){
            if(writingFileNo ==1){
                add_token(&tokens1, token);
            }else if(writingFileNo==2){
                add_token(&tokens2, token);
            }
            fprintf(output_file, "%s\n", token);
            return;
        }
        //기타(printf와 같은 기본 함수)
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }
        fprintf(output_file, "%s\n", token);
        return;
    }
    //기타 # 포함

        // {( [ ]) }이면
    else if(
        (strncmp(token, "{", 1) ==0) ||
        (strncmp(token, "}", 1)==0)||
        (strncmp(token, ")", 1)==0)||
        (strncmp(token, "(", 1)==0)||
        (strncmp(token, "[", 1)==0)||
        (strncmp(token, "]", 1)==0)
    ){
        // 뒤에 더 있으면
        if(strlen(token)!=1){
            int start =1;
            //공백이 아닌 부분을 찾기
            for(int i=1; i<strlen(token); i++){
                if(token[i] != ' '){
                    start = i;
                    break;
                }
            }
            char* substring = (char*)malloc(strlen(token)- start + 1);
            strcpy(substring, token + start);
            substring[strlen(token)- start] = '\0';
            tokenize(substring);
            }
        return;
    }
    //# define, # include 등
    else if(token[0]=='#'){
        
        // # 단독일 때
        if(strlen(token) ==1 ){
            if(writingFileNo ==1){
                add_token(&tokens1, token);
            }else if(writingFileNo==2){
                add_token(&tokens2, token);
            }
            fprintf(output_file, "%s\n", token);
            isPri =0;
            wassharp = -1;
        }
        else{ //#define 처럼 붙어서 나왔을 때
        
            if(writingFileNo ==1){
                add_token(&tokens1, token);
            }else if(writingFileNo==2){
                add_token(&tokens2, token);
            }
            fprintf(output_file, "%s\n", token);
            isPri =0;
            wassharp =1;
        }
        return;
        
    }
    else{
        
        if(writingFileNo ==1){
            add_token(&tokens1, token);
        }else if(writingFileNo==2){
            add_token(&tokens2, token);
        }
        fprintf(output_file, "%s\n", token);
        return;
    }
}

void lineControl(char line[], int isAnnotation){
    // printf("%s 를 라인 컨트롤  %d define: %d\n ",line, isAnnotation, isDefineLine);
    char *token = line;
    int lineLength=0;
    int semicolon =0;
    int comma =0;

    //#define의 경우

    char * preptr= malloc(strlen(line)+1);
    preptr[strlen(line)] = '\0';
    int preCount = 1;//while 문 안에서 반복 횟수를 셀 변수
    strcpy(preptr, line);
    //공백으로 자르기
    char * pretoken = strtok(preptr, " ");
        // #define으로 시작하는 경우
        if(preptr[0] =='#' && (strncmp(preptr+1, "define", 6) == 0)){

        while (pretoken != NULL) {
                isDefineLine =1;
                if (preCount == 2 || preCount == 3) {
                    // 해당 조각에 '(' 문자가 있는지 확인
                    if (strchr(pretoken, '(') != NULL) {
                        //preCount//현재 괄호가 있는 토큰
                        defineFunc = -1;
                        break;
                    }
                }
            preCount ++;
            //다음 토큰 만들기
            pretoken = strtok(NULL, " ");
            }
    }
     // # define으로 시작하는 경우
    else if(preptr[0] =='#' && (strncmp(preptr+2, "define", 6) == 0)){

        while (pretoken != NULL) {
                isDefineLine =1;
                if (preCount == 3 || preCount == 4) {
                    // 해당 조각에 '(' 문자가 있는지 확인
                    if (strchr(pretoken, '(') != NULL) {
                        //preCount//현재 괄호가 있는 토큰
                        defineFunc = -1;
                        break;
                    }
                }
            preCount ++;
            //다음 토큰 만들기
            pretoken = strtok(NULL, " ");
            }
    }


    // if(isAnnotation == 0 || isAnnotation == -1){ //만약 주석이 아니거나 존재하는지 모르면 short와 long은 0으로 초기화
    //     isLongAnnotation =0;
    // }
    
    //한 줄의 길이 찾기 및 세미콜론, 콤마 찾기
    for(int i=0; i<strlen(line); i++){
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
    //주석인지 확인했는데 주석이면
    //긴 주석만 하면 됨(짧은 주석은 memmove으로 처리)
    if(isLongAnnotation ==1){ // /*를 통해서 주석이 시작되었으면
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '*' && line[i+1] == '/'){ // */를 찾으면
                if(lineLength == i+2){// /*만 있는 줄이면
                    isLongAnnotation =0;
                    return;
                }
                 memmove(line, line+i+2, lineLength-i-1);//그 뒤부터 시작되게 line을 변경
                 isLongAnnotation =0;
                 lineControl(line, -1); //바뀐 라인을 다시 컨트롤
                 return;
            }
        }
        // */을 못찾았으면
        return;
    }

    //아직 주석인지 확인을 안했으면
    else if(isAnnotation == -1){
        //아직 주석이 안끝났으면
        if(isLongAnnotation == 1) return;
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '/'){
                if(line[0] == '/' && line[1] == '/'){ // //로 시작하는 주석만있는 줄이면 
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
                    isLongAnnotation = 1; //긴 주석 시작
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
            else if(line[lineLength-1] == '{' || line[lineLength] == '{'){
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
                    strncmp(token, "enum", 4) ==0 ||
                    strncmp(token, "void", 4) == 0 || 
                    strncmp(token, "FILE", 4) == 0 ||
                    strncmp(token, "struct", 6) == 0 ||
                    strncmp(token, "typedef", 7) == 0 ||
                    strncmp(token, "union", 5) == 0
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
                        //공백으로 나누기
                        line[ lineLength-1] = ' ';
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
            //기타 #포함
            else{
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


    char *filename1 = argv[1];
    char *filename2 = argv[2];
/*
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");

    char line[MAX_LINE_LENGTH];
        while (fgets(line, MAX_LINE_LENGTH, file1) != NULL) {
         
            printf("%s", line);
        }

    fclose(file1);
    fclose(file2);
*/
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");
//    FILE *file1 = fopen("input.c", "r");
//    FILE *file2 = fopen("input2.c", "r");
    char line[MAX_LINE_LENGTH];
    FILE *output_file = fopen("hw2_output.txt", "w");
    fprintf(output_file, "%s", "");
    // FILE *output_file = fopen("output.txt", "a");
    writingFileNo =1;
    while (fgets(line, MAX_LINE_LENGTH, file1) != NULL) {
        isDefineLine =0;
        lineControl(line, -1);
    }
    //별 출력
    printStar =1;
    tokenize("a");

    writingFileNo =2;
        // fprintf(output_file, "%s\n", "*****");
    while (fgets(line, MAX_LINE_LENGTH, file2) != NULL) {
        isDefineLine =0;
        lineControl(line, -1);
    
    }
    //별 출력
    printStar =1;
    tokenize("a");
    //lcs 출력
    printStar =10;
    tokenize("a");
    //별 출력
    printStar =1;
    tokenize("a");
    //표절율 출력
    printStar =11;
    tokenize("a");

    fclose(file1);
    fclose(file2);
    return 0;
}