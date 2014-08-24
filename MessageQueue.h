#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include <vector>

#include "Message.h"

class MessageQueue {
    public:
        void Append(Message message) { messages.push_back(message); }
    private:
        std::vector<Message> messages;
};

#endif  // MESSAGEQUEUE_H_
