#ifndef _HOOK_H
#define _HOOK_H

class Hook {
    public:
        void* func;
        bool scheme;
        Hook(void* func, bool scheme = false);
};

#endif