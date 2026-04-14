#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "ui.h"
#include "perms.h"

static void print_perms(perms_t mode) {
    char symbolic[10];
    char binary[10];
    int octal;
    
    perms_to_symbolic(mode, symbolic);
    perms_to_binary(mode, binary);
    octal = perms_to_octal(mode);
    
    printf("  Symbolic: %s\n", symbolic);
    printf("  Octal:    %03o\n", octal);
    printf("  Binary:   %s\n", binary);
}

static void mode_1_string_input(void) {
    char input[32];
    perms_t mode;
    
    printf("\nMode 1: Enter permissions as string\n");
    printf("Enter permissions (755 or rwxr-xr-x): ");
    if (!fgets(input, sizeof(input), stdin)) return;
    
    input[strcspn(input, "\n")] = '\0';
    
    if (perms_parse_string(input, &mode) != 0) {
        printf("Error: invalid format.\n");
        return;
    }
    
    printf("Result:\n");
    print_perms(mode);
}

static void mode_2_file_stat(void) {
    char pathname[256];
    struct stat sb;
    
    printf("\nMode 2: File information\n");
    printf("Enter file path: ");
    if (!fgets(pathname, sizeof(pathname), stdin)) return;
    
    pathname[strcspn(pathname, "\n")] = '\0';
    
    if (stat(pathname, &sb) != 0) {
        printf("Error: cannot read file.\n");
        return;
    }
    
    printf("Permissions for '%s':\n", pathname);
    print_perms(sb.st_mode & 0777);
}

static void mode_3_modify_perms(void) {
    char perms_str[32];
    char modifier[256];
    perms_t current_mode;
    perms_t new_mode;
    
    printf("\nMode 3: Modify permissions\n");
    printf("Enter current permissions (755 or rwxr-xr-x): ");
    if (!fgets(perms_str, sizeof(perms_str), stdin)) return;
    
    perms_str[strcspn(perms_str, "\n")] = '\0';
    
    if (perms_parse_string(perms_str, &current_mode) != 0) {
        printf("Error: invalid format.\n");
        return;
    }
    
    printf("Enter modifier (u+x,g-w,o=r): ");
    if (!fgets(modifier, sizeof(modifier), stdin)) return;
    
    modifier[strcspn(modifier, "\n")] = '\0';
    
    if (perms_apply_modifier(current_mode, modifier, &new_mode) != 0) {
        printf("Error: invalid modifier.\n");
        return;
    }
    
    printf("Before:\n");
    print_perms(current_mode);
    printf("\nAfter:\n");
    print_perms(new_mode);
}

void ui_run_menu(void) {
    while (1) {
        printf("\n=== PERMISSION MASK ===\n");
        printf("1. Enter permissions as string\n");
        printf("2. File information\n");
        printf("3. Modify permissions\n");
        printf("0. Exit\n");
        printf("> ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Input error.\n");
            continue;
        }
        while (getchar() != '\n');
        
        switch (choice) {
            case 1:
                mode_1_string_input();
                break;
            case 2:
                mode_2_file_stat();
                break;
            case 3:
                mode_3_modify_perms();
                break;
            case 0:
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}
