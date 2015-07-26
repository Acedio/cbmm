#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include <list>

#include "Message.h"

class MessageQueue {
 public:
  void Append(Message message) { messages.push_back(message); }
  std::list<Message>::const_iterator GetIter() { return messages.cbegin(); }

 private:
  std::list<Message> messages;
};

#endif  // MESSAGEQUEUE_H_
