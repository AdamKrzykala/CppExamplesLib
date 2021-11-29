#include <atomic>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>


/***
 *      customQueue class
 *      Class created for designing thread-safe queue 
 *      which can be able to be shared
 *      @tparam T the type of data stored in queue
 */
template<class T>
class customQueue
{
private:
    std::queue<T> _queue;
    size_t _queueMaxSize;

    mutable std::mutex m;
    std::condition_variable cond_not_empty;
    std::condition_variable cond_not_full;

public:
    customQueue(size_t maxSize) : _queueMaxSize(maxSize) {};

    customQueue(const customQueue &queue) = default;
    customQueue& operator=(customQueue queue) { 
        std::swap(_queue, queue._queue);
        std::swap(_queueMaxSize, queue._queueMaxSize);
        return *this;
    }

    customQueue(customQueue&& queue) = delete;
    customQueue& operator=(customQueue&& queue) = delete;

    virtual ~customQueue() {};

    // Put, Get and GetCount
    // Standard methods for queue
    bool Put(T);
    std::unique_ptr<T> Get();
    size_t GetCount();

    //Put, Get
    // Methods for thread usage
    std::unique_ptr<T> getToThread(std::atomic_bool *);
    bool putFromThread(T, std::atomic_bool *);
    void prepareToEnd();
};

/***
 *      GetCount() function
 *      Task: returning amount of elements in queue
 *      @tparam T type of element stored in queue,
 *      @return amount of elements stored in queue
 */
template<class T>
size_t customQueue<T>::GetCount()
{
    return _queue.size();
}

/***
 *      Put() function
 *      Task: Simple adding new element to queue
 *      @tparam T type of element stored in queue,
 *      @return true if adding was succesful,
 *      @return false if adding was failure
 * 
 *      @warning failure may occure when the queue is full
 */
template<class T>
bool customQueue<T>::Put(T newElement)
{
    if (_queue.size() >= _queueMaxSize) {
        return false;
    }
    _queue.push(newElement);
    
    return true;
}

/***
 *      Get() function
 *      Task: Simple getting next element from queue
 *      @tparam T type of element stored in queue,
 *      @return nullptr if queue is empty (no element),
 *      @return unique_ptr to next element from queue
 * 
 *      @warning nullptr is returned when the queue is empty
 */
template<class T>
std::unique_ptr<T> customQueue<T>::Get()
{
    if (!_queue.empty()) {
        T element = _queue.front();
        _queue.pop();
        return std::make_unique<T>(std::move(element));
    }

    return nullptr;
}

/***
 *      getToThread() function
 *      Task: Getting new element from queue to thread
 *      @tparam T type of element stored in queue,
 *      @return unique_ptr to next element from queue,
 * 
 *      @warning if queue is empty, the function waits
 */
template<class T>
std::unique_ptr<T> customQueue<T>::getToThread(std::atomic_bool *end) 
    {
        std::unique_lock<std::mutex> lock(m);
        while(_queue.empty() && !*end)
        {
            cond_not_empty.wait(lock);
        }
        auto element = Get();
        cond_not_full.notify_one();
        return element;
    }

/***
 *      putFromThread() function
 *      Task: Adding new element to queue
 *      @tparam T type of element stored in queue,
 *      @return true if element is added,
 *      @return false if element could not be added,
 * 
 *      @warning if queue is full, the function waits
 */
template<class T>
bool customQueue<T>::putFromThread(T newElement, std::atomic_bool *end)
{
    std::unique_lock<std::mutex> lock(m);
    while(_queue.size() >= _queueMaxSize && !*end)
    {
        std::cerr << "Queue is full. Waiting for free space... \n";
        cond_not_full.wait(lock);
    }
    bool elementAdded = Put(newElement);
    cond_not_empty.notify_one();
    return elementAdded;
}

/***
 *      prepareToEnd() function
 *      Task: Helper function to end all of 
 *          the waiting condition_variables
 */
template<class T>
void customQueue<T>::prepareToEnd(){
    cond_not_full.notify_all();
    cond_not_empty.notify_all();
}


