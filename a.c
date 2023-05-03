#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000

int static annotationStart = 0; // /*를 통해서 주석이 시작하면 1이됨
char funcTable[100][100];
char varTable[100][100];
int static funcTableSize = 0;
int static varTableSize = 0;
int static isPri =0;
int static isFisrtFunc=0;
int static isFirstString =0;
int static isAnnotation = -1; //-1이면 아직 확인 안한거, 0이면 확인했는데 아닌거, 1이면 주석인거
void lineControl(char line[]){
    printf("%s 를 라인 컨트롤 \n",line);
    char *token = line;
    int lineLength=0;
    int semicolon =0;
    int comma =0;
    annotationStart =-1;
    //한 줄의 길이 찾기 및 세미콜론, 콤마 찾기
    for(int i=0; i<MAX_LINE_LENGTH; i++){
        if(line[i] == '\n'){
            lineLength = i;
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
    //주석인지 확인
    if(annotationStart ==1){ // /*를 통해서 주석이 시작되었으면
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '*' && line[i+1] == '/'){ // */를 찾으면
                 memmove(line, line+i+2, lineLength-i-1);//그 뒤부터 시작되게 line을 변경
                 annotationStart ==0;
                 lineControl(line); //바뀐 라인을 다시 컨트롤
                 return;
            }
        }
        if (annotationStart == 1) return; //아직 주석 끝이 안나왔으면
    }
    //아직 주석인지 확인을 안했으면
    else if(isAnnotation == -1){
        for(int i=0 ; i<lineLength; i++){
            if(line[i] == '/'){
                if(line[i+1] == '/'){// //가 있으면
                line[i] = '\0'; // //전까지만 가리키도록 함
                printf("line: %s\n", line);
                lineControl(line); //바뀐 라인을 다시 컨트롤
                return;
                }else if (line[i+1] == '*'){ // /*가 있으면
                    isAnnotation = 1;
                    line[i] = '\0'; // /*전까지만 가리키도록 함
                    lineControl(line); //바뀐 라인을 다시 컨트롤
                printf("line: %s\n", line);
                    printf("다시 라닝 컨트롤 ㄱ ㄱ");
                    return;
                }
            } 
        }
        //주석이 없고 다시 라인 컨트롤
        isAnnotation = 0;
        lineControl(line);
    }