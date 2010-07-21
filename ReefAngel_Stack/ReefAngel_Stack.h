/*
 * Copyright 2010 / Curt Binder
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REEFANGEL_STACK_H__
#define __REEFANGEL_STACK_H__

//#include <wiring.h>
#include <stdint.h>

/*
class ReefAngel_IntNodeClass
{
    public:
        int data;
        ReefAngel_IntNodeClass *link;
};

class ReefAngel_IntStackClass
{
    public:
        ReefAngel_IntStackClass() { top = 0; }
        ~ReefAngel_IntStackClass();
        bool IsEmpty() const { return top == 0; }
        int Top() const { return top->data; }
        ReefAngel_IntStackClass& Push(const int& x);
        ReefAngel_IntStackClass& Pop(int& x);
    private:
        ReefAngel_IntNodeClass *top;  //pointer to top node
};
*/

class ReefAngel_ByteNodeClass
{
    public:
        uint8_t data;
        ReefAngel_ByteNodeClass *link;
};

class ReefAngel_ByteStackClass
{
    public:
        ReefAngel_ByteStackClass() { top = 0; }
        ~ReefAngel_ByteStackClass();
        bool IsEmpty() const { return top == 0; }
        //uint8_t Top() const { return top->data; }
        ReefAngel_ByteStackClass& Push(const uint8_t& x);
        ReefAngel_ByteStackClass& Pop(uint8_t& x);
        void Clear();
    private:
        ReefAngel_ByteNodeClass *top;  //pointer to top node
};

#endif  // __REEFANGEL_STACK_H__
