#include "../../include/Cache/TimerManager.h"
#include "../../include/SearchEngineServer/Configuration.h"
#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>
#include "../../include/Cache/CacheTimerTask.h"


TimerManager *TimerManager::_tmanager = nullptr;
pthread_once_t TimerManager::_once = PTHREAD_ONCE_INIT;


TimerManager* TimerManager::getTimerManager()
{
    pthread_once(&_once, init);
    return _tmanager;
}
void TimerManager::init()
{
    _tmanager = new TimerManager();
    atexit(destroy);
}
void TimerManager::destroy()
{
    if(_tmanager)
    {
        delete _tmanager;
        _tmanager = nullptr;
    }
}


TimerManager::TimerManager()
: _timerfd(createTimerfd())
, _isStarted(false)
{
    map<string, string> &cmap = Configuration::getInstance()->getConfigMap();
    initTimer(stoi(cmap["timer_delay"]), stoi(cmap["timer_interval"]));
}

void TimerManager::initTimer(int delay, int interval)
{
    _delay = delay;
    _interval = interval;
}
/*
## poll

### 主旨思想

- 用一个结构体记录文件描述符集合，并记录用户态状态和内核态状态

### 函数说明

- 概览

  ```c++
  #include <poll.h> 
  struct pollfd { 
      int fd;  //委托内核检测的文件描述符 
      short events;  //委托内核检测文件描述符的什么事件  
      short revents;  //文件描述符实际发生的事件 
  };
  
  int poll(struct pollfd *fds, nfds_t nfds, int timeout);
  ```

- `int poll(struct pollfd *fds, nfds_t nfds, int timeout); `

  - 通过`man poll`查看帮助
  - 参数
    - `fds`：是一个`struct pollfd` 结构体数组，这是一个需要检测的文件描述符的集合
    - `nfds`：这个是第一个参数数组中最后一个有效元素的下标 + 1 
    - `timeout`：阻塞时长 
      - 0：不阻塞
      - -1：阻塞，当检测到需要检测的文件描述符有变化，解除阻塞
      - \>0：具体的阻塞时长(ms)
  - 返回值
    - -1：失败
    - \>0(n)：检测的集合中有n个文件描述符发生了变化
*/
void TimerManager::start()
{
    struct pollfd pfd;
    pfd.fd = _timerfd;
    pfd.events = POLLIN;

    CacheTimerTask task;
    _tmanager->attach(&task);

    setTimerfd(_delay, _interval);

    _isStarted = true;
    while(_isStarted)
    {
        int nready = poll(&pfd, 1, 5000);
        if (-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            perror("-1 == nready");
            return;
        }
        else if(0 == nready)
        {
            continue;
        }
        else
        {
            if(pfd.revents & POLLIN)
            {
                // 先处理读事件
                handleRead();
                // 再去执行任务
                for(auto &task : _wheelList)
                {
                    task->process();
                }
            }
        }
    }
    return;
}
void TimerManager::stop()
{
    _isStarted = false;
    setTimerfd(0, 0); // 关闭定时器
    return;
}

void TimerManager::attach(TimerTask *task)
{
    auto it = find(_wheelList.begin(), _wheelList.end(), task);
    if(it == _wheelList.end())
        _wheelList.push_back(task);
    return;
}
void TimerManager::detach(TimerTask *task)
{
    auto it = find(_wheelList.begin(), _wheelList.end(), task);
    if(it != _wheelList.end())
        _wheelList.erase(it);
    return;
}

int TimerManager::createTimerfd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(-1 == fd)
    {
        perror("timerfd_create");
        return -1;
    }
    return fd;
}

void TimerManager::setTimerfd(int delay, int interval)
{
    struct itimerspec value;
    value.it_value.tv_sec = delay;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = interval;
    value.it_interval.tv_nsec = 0;

    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(-1 == ret)
    {
        perror("timerfd_settime");
    }

}

void TimerManager::handleRead()
{
    uint64_t value;
    int ret = read(_timerfd, &value, sizeof(value));
    if(ret != sizeof(value))
    {
        perror("read");
        return;
    }
}
