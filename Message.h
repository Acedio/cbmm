#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "MessageType.h"

class Message {
    public:
        MessageType GetType() { return type_; }
    private:
        MessageType type_;
};

#endif  // MESSAGE_H_
