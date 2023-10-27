#include "hook.h"

Hook::Hook(void* func, bool scheme) {
    this->func = func;
    this->scheme = scheme;
}