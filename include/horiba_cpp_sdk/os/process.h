#ifndef PROCESS_H
#define PROCESS_H

namespace horiba::os {

class Process {
 public:
  virtual ~Process() = default;

  virtual void start() = 0;
  virtual bool running() = 0;
  virtual void stop() = 0;
};

} /* namespace horiba::os */
#endif /* ifndef PROCESS_H */
