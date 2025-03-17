#include "birthday_present.h"
#include <stdio.h>

void birthday_present()
{

    
    char *message = "Happy Birthday to dear 43-year old!!!\n"
                    "As a birthday present, I will provide you the following services:\n"
                    "- 3 times I will prepare the food you want\n"
                    "- 1 time I will go to you with a restaurant you want to go to";                    

                    // Define the file path
                    const char *file_path = "/home/juhohe/Desktop/miias_gift.txt";

                    // Open the file for writing
                    FILE *file = fopen(file_path, "w");
                    if (file == NULL) {
                        perror("Error opening file");
                        return;
                    }

                    // Write the message to the file
                    fprintf(file, "%s", message);

                    // Close the file
                    fclose(file);

                    printf("Message written to %s\n", file_path);
}