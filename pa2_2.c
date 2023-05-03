#include <stdio.h>
#include <string.h>

// 토큰 종류를 나타내는 열거형
enum token_type
{
    INVALID,   // 잘못된 토큰
    NUM,       // 숫자
    VAR,       // 변수
    STR,       // 문자열
    FUNC,      // 함수
    SEMICOLON, // 세미콜론
    COMMA,     // 콤마
    LPAREN,    // 왼쪽 괄호
    RPAREN,    // 오른쪽 괄호
    HASH,      // 해시 기호
    INCLUDE,   // include 키워드
};

// 토큰 구조체
struct token
{
    enum token_type type; // 토큰의 종류
    char value[100];      // 토큰의 값
};

// isspace 함수 구현
int my_isspace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

// isdigit 함수 구현
int my_isdigit(char c) {
    return (c >= '0' && c <= '9');
}

// isalpha 함수 구현
int my_isalpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

// isalnum 함수 구현
int my_isalnum(char c) {
    return (my_isalpha(c) || my_isdigit(c));
}

// 토큰을 분리하는 함수
struct token get_next_token(char *line)
{
    struct token token;
    int i = 0;
    int len = strlen(line);

    // 토큰의 값 초기화
    memset(token.value, 0, sizeof(token.value));

    // 공백 문자 건너뛰기
    while (i < len && my_isspace(line[i]))
    {
        i++;
    }

    // 라인의 끝까지 탐색
    while (i < len)
    {
        if (my_isdigit(line[i]))
        { // 숫자일 경우
            token.type = NUM;
            while (i < len && my_isdigit(line[i]))
            {
                token.value[strlen(token.value)] = line[i];
                i++;
            }
            break;
        }
        else if (my_isalpha(line[i]))
        { // 문자일 경우
            token.type = VAR;
            while (i < len && my_isalnum(line[i]))
            {
                token.value[strlen(token.value)] = line[i];
                i++;
            }
            break;
        }
        else if (line[i] == '\"')
        { // 문자열일 경우
            token.type = STR;
            i++;
            while (i < len && line[i] != '\"')
            {
                token.value[strlen(token.value)] = line[i];
                i++;
            }
            break;
        }
        else if (line[i] == '(')
        { // 왼쪽 괄호일 경우
            token.type = LPAREN;
            token.value[0] = '(';
            i++;
            break;
        }
        else if (line[i] == ')')
        { // 오른쪽 괄호일 경우
            token.type = RPAREN;
            token.value[0] = ')';
            i++;
            break;
        }
        else if (line[i] == ',')
        { // 콤마일 경우
            token.type = COMMA;
            token.value[0] = ',';
            i++;
            break;
        }
        else if (line[i] == ';')
        { // 세미콜론일 경우
            token.type = SEMICOLON;
            token.value[0] = ';';
            i++;
            break;
        }
        else if (line[i] == '#')
        { // 해시 기호일 경우
            token.type = HASH;
            token.value[0] = '#';
            i++;
            break;
        }
        else if (my_isspace(line[i]))
        { // 공백 문자일 경우
            // 다음 토큰 탐색을 위해 다음 문자로 이동
            i++;
            continue;
        }

        else if (line[i] == '/')
        { // 주석일 경우
            if (i + 1 < len && line[i + 1] == '/')
            { // 라인 주석
                break;
            }
            else if (i + 1 < len && line[i + 1] == ' ')
            { // 블록 주석
                i += 2;
                while (i < len)
                {
                    if (line[i] == ' ' && i + 1 < len && line[i + 1] == '/')
                    {
                        i += 2;
                        break;
                    }
                    i++;
                }
                break;
            }
            else
            { // 나머지 경우는 연산자로 처리
                token.type = INVALID;
                token.value[strlen(token.value)] = line[i];
                i++;
                break;
            }
        }
        else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/')
        {
            // 연산자일 경우
            token.type = INVALID;
            token.value[strlen(token.value)] = line[i];
            i++;
            break;
        }
        else
        { // 나머지 경우는 잘못된 토큰으로 처리
            token.type = INVALID;
            token.value[strlen(token.value)] = line[i];
            i++;
            break;
        }
    }
    return token;
}
int main()
{
    char line[1000];
    struct token token;
    enum token_type last_token_type = INVALID;
    // 파일 포인터 생성
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("output.txt", "w");

    // 파일을 한 줄씩 읽어서 토큰 분리
    while (fgets(line, sizeof(line), input_file))
    {
        // 토큰 분리
        char *ptr = line;
        while (*ptr != '\0')
        {
            struct token token = get_next_token(ptr);
            ptr += strlen(token.value);
            printf("토크니 %s\n", token.value);
            // include 키워드인 경우
            if (last_token_type == HASH && token.type == VAR && strcmp(token.value, "include") == 0)
            {
                fprintf(output_file, "%s ", token.value);
            }
            else if (last_token_type == HASH && token.type == STR)
            {
                // include 파일 이름인 경우
                fprintf(output_file, "%s\n", token.value);
            }
            else if (token.type != INVALID && token.type != HASH)
            {
                // 그 외의 경우
                fprintf(output_file, "%s ", token.value);
            }

            // 이전 토큰 종류 저장
            last_token_type = token.type;
        }
    }

    // 파일 포인터 닫기
    fclose(input_file);
    fclose(output_file);

    return 0;
}