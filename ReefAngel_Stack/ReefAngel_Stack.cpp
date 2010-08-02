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

#include "ReefAngel_Stack.h"
#include <stdlib.h>

/*
ReefAngel_IntStackClass::~ReefAngel_IntStackClass()
{
    //destructor
    ReefAngel_IntNodeClass *next;
    while (top)
    {
        next = top->link;
        free(top);
        top = next;
    }//end while
}

ReefAngel_IntStackClass& ReefAngel_IntStackClass::Push(const int& x)
{
    //adds element x to top of stack
    ReefAngel_IntNodeClass *p = (ReefAngel_IntNodeClass*)malloc(sizeof(ReefAngel_IntNodeClass));
    p->data = x;
    p->link = top;
    top = p;
    return *this;
}

ReefAngel_IntStackClass& ReefAngel_IntStackClass::Pop(int& x)
{
    //deletes the top element and puts it in x
    x = top->data;
    ReefAngel_IntNodeClass *p = top;
    top = top->link;
    free(p);
    return *this;
}
*/

ReefAngel_ByteStackClass::~ReefAngel_ByteStackClass()
{
    //destructor
    ReefAngel_ByteNodeClass *next;
    while (top)
    {
        next = top->link;
        free(top);
        top = next;
    }//end while
}

ReefAngel_ByteStackClass& ReefAngel_ByteStackClass::Push(const uint8_t& x)
{
    //adds element x to top of stack
    ReefAngel_ByteNodeClass *p = (ReefAngel_ByteNodeClass*)malloc(sizeof(ReefAngel_ByteNodeClass));
    p->data = x;
    p->link = top;
    top = p;
    return *this;
}

ReefAngel_ByteStackClass& ReefAngel_ByteStackClass::Pop(uint8_t& x)
{
    //deletes the top element and puts it in x
    if ( IsEmpty() )
    {
        // can't pop the top, so set the value to 0 (or maybe 255)
        x = 255;
    }
    else
    {
        x = top->data;
        ReefAngel_ByteNodeClass *p = top;
        top = top->link;
        free(p);
    }
    return *this;
}

void ReefAngel_ByteStackClass::Clear()
{
    uint8_t x;
    while ( ! IsEmpty() )
    {
        Pop(x);
    }  // while ! IsEmpty()
}
