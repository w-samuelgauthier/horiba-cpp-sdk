#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

namespace horiba::communication {

class Command;
class Response;

class Communicator {
 public:
  virtual ~Communicator() = 0;

  virtual void open() = 0;
  virtual void close() = 0;
  virtual Response request_with_response(Command command) = 0;
};

}  // namespace horiba::communication
#endif /* ifndef COMMUNICATOR */
