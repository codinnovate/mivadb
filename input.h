#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

void readLine(const char *prompt, char *buffer, size_t size);
int readInt(const char *prompt);
float readFloat(const char *prompt);

#endif /* INPUT_H */
