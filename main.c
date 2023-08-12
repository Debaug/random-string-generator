#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define LOWERCASES "abcdefghijklmnopqrstuvwxyz"
#define UPPERCASES "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"
#define SPECIALS "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"

#define LOWERCASE_COUNT 26
#define UPPERCASE_COUNT 26
#define DIGIT_COUNT 10
#define SPECIAL_COUNT 32

typedef struct Flags {
    bool lowercases;
    bool uppercases;
    bool digits;
    bool specials;
} Flags;

size_t prompt_len(void);
Flags prompt_flags(void);

typedef int FlagsFromInputError;

#define SUCCESS 0
#define ERROR_EMPTY -1
#define ERROR_INVALID_CHAR -2

typedef struct FlagsFromInputResult {
    FlagsFromInputError error;
    char invalid_char;
} FlagsFromInputResult;

FlagsFromInputResult flags_from_input(const char* input, Flags* p_flags);

void random_string(Flags flags, char* dst, size_t len);
#define CHAR_SET_BUF_LEN LOWERCASE_COUNT + UPPERCASE_COUNT + DIGIT_COUNT + SPECIAL_COUNT
size_t create_char_set(Flags flags, char char_set[CHAR_SET_BUF_LEN]);
char random_char_from_set(const char* char_set, size_t len);

int main(int argc, const char* argv[]) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);

    bool quit = false;
    bool prompt = true;
    size_t len;
    Flags flags;

    while (!quit) {
        if (prompt) {
            len = prompt_len();
            flags = prompt_flags();
        }

        char* str = malloc(len + 1);
        random_string(flags, str, len);
        printf("\nGenerated a random string: %s\n", str);
        free(str);

        printf("\nEnter 's' to start over, 'r' recreate, or anything else to exit:  ");
        char response[3] = {0};
        fgets(response, 3, stdin);
        if (strcmp(response, "s\n") == 0) prompt = true;
        else if (strcmp(response, "r\n") == 0) prompt = false;
        else quit = true;
    }

    return EXIT_SUCCESS;
}

#define MAX_LEN_INPUT_LEN 20

size_t prompt_len(void) {
    printf("Please enter the desired string length:  ");
    char input[MAX_LEN_INPUT_LEN + 1];

    size_t len;
    do {
        fgets(input, MAX_LEN_INPUT_LEN + 1, stdin);
        len = atoll(input);
        if (len == 0) {
            printf("Invalid string length, please try again:  ");
        }
    } while (len == 0);

    return len;
}

#define MAX_FLAG_INPUT_LEN 64

Flags prompt_flags(void) {
    printf(
        "Please enter 'a' to use lowercase letters, 'A' for uppercase letters, '1' for digits, '&' for special characters, or any combination:  "
    );
    char input[MAX_FLAG_INPUT_LEN + 1];

    Flags flags;
    FlagsFromInputResult result;
    do {
        fgets(input, MAX_FLAG_INPUT_LEN + 1, stdin);
        result = flags_from_input(input, &flags);

        switch (result.error) {
        case ERROR_EMPTY:
            printf("Input can't be empty, please try again:  ");
            break;
        case ERROR_INVALID_CHAR:
            printf("`%c` is not a valid input character, please try again:  ", result.invalid_char);
            break;
        default: break;
        }
    } while (result.error != SUCCESS);

    return flags;
}

FlagsFromInputResult flags_from_input(const char* input, Flags* p_flags) {
    if (input[0] == '\n') return (FlagsFromInputResult) { ERROR_EMPTY, '\0' };

    Flags flags = { false, false, false, false };

    for (size_t i = 0; input[i] != '\0'; i++) {
        switch (input[i]) {
        case 'a':
            flags.lowercases = true;
            break;
        case 'A':
            flags.uppercases = true;
            break;
        case '1':
            flags.digits = true;
            break;
        case '&':
            flags.specials = true;
            break;
        case '\n': break;
        default:
            return (FlagsFromInputResult) { ERROR_INVALID_CHAR, input[i] };
        }
    }

    *p_flags = flags;
    return (FlagsFromInputResult) { SUCCESS, '\0' };
}

void random_string(Flags flags, char* dst, size_t len) {
    char char_set[CHAR_SET_BUF_LEN];
    size_t char_set_len = create_char_set(flags, char_set);

    for (int i = 0; i < len; i++) {
        dst[i] = random_char_from_set(char_set, char_set_len);
    }
    dst[len] = '\0';
}

size_t create_char_set(Flags flags, char char_set[CHAR_SET_BUF_LEN]) {
    size_t len = 0;

    if (flags.lowercases) {
        memcpy(char_set + len, LOWERCASES, LOWERCASE_COUNT);
        len += LOWERCASE_COUNT;
    }
    if (flags.uppercases) {
        memcpy(char_set + len, UPPERCASES, UPPERCASE_COUNT);
        len += UPPERCASE_COUNT;
    }
    if (flags.digits) {
        memcpy(char_set + len, DIGITS, DIGIT_COUNT);
        len += DIGIT_COUNT;
    }
    if (flags.specials) {
        memcpy(char_set + len, SPECIALS, SPECIAL_COUNT);
        len += SPECIAL_COUNT;
    }

    return len;
}

char random_char_from_set(const char* char_set, size_t len) {
    size_t index = (double)rand() * ((double)len / (double)RAND_MAX);
    return char_set[index];
}
