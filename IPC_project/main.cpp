#include <string>
#include <thread>

#include "customQueue.hpp"

/***
 *      Program designed for writing and reading from queue.
 *      A queue is shared within two threads.
 *      First thread adds strings to queue up to its capacity
 *      Second thread takes strings from queue and prints it
 *      Q - taking and printing one string from queue
 *      X - ending program execution
 *  
 *      Date: 14.10.2021
 *      Author: Adam Krzykala\
 *      @param[in] queueCapacity capacity of shared queue,
 *      @param[in] InputArray array with input text
 */


//Predefined queue capacity
constexpr size_t queueCapacity = 3;

// Predefined InputArray with text Lorem Ipsum
const char InputArray[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.Donec sollicitudin neque quis massa aliquet interdum. Suspendisse semper, justo non vehicula fermentum, velit tortor tempor eros, non scelerisque lacus eros id odio. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed mi lorem, sodales ac dignissim vitae, mollis et elit. Proin convallis sem vel dui fermentum, quis elementum dolor volutpat. Vivamus aliquam egestas scelerisque. Nam pellentesque viverra nisi, vel mollis nisi varius in. Aliquam hendrerit elementum ipsum, sed scelerisque nisi imperdiet in. Pellentesque a risus non nulla pulvinar ornare. Donec sit amet tortor ac ex suscipit sodales. Fusce porttitor convallis dolor eu congue. ";

/***
 *      --- Reader function ---
 *      Task: reading from defined customQueue
 * 
 *      @tparam T the type of data stored in queue,
 *      @param sharedQueue Queue shared between threads,
 *      @param end Atomic bool to stop program when user chooses 'X' option
 */
template<typename T>
void reader(std::shared_ptr<customQueue<T>> sharedQueue, std::atomic_bool *end)
{
    char check = 'x';
    while (check != 'X'){

        //Getting new string from queue when user chooses 'Q'
        if(check == 'Q' && sharedQueue->GetCount()){
            auto element = sharedQueue->getToThread(end);
                if (element) {
                    std::cout << "#" << sharedQueue->GetCount() << " " << *element << "\n";
                }
                else {
                    std::cerr << "Queue is empty! \n";
                }
	    }

	    std::cin >> check;

	    //Ending when user chooses 'X'
        if (check == 'X'){
            *end = true;
            sharedQueue->prepareToEnd();
            break;
        }
    }
    std::cout << "Reader ended" << std::endl;
}

/***
 *      --- Writer function ---
 *      Task: writing from defined customQueue
 * 
 *      @tparam T the type of data stored in queue,
 *      @param sharedQueue Queue shared between threads,
 *      @param end Atomic bool to stop program when user chooses 'X' option
 */
template<typename T>
void writer(std::shared_ptr<customQueue<T>> sharedQueue, std::atomic_bool *end)
{
    std::string source(InputArray);

    //End of sentence delimiter
    std::string delimiter = ". ";
    size_t pos = 0;
    std::string token;
    while ((pos = source.find(delimiter)) != std::string::npos){
        token = source.substr(0, pos);
	    sharedQueue->putFromThread(token, end);
        source.erase(0, pos + delimiter.length());
	    if (*end) break;
    }

    std::cout << "Writer ended" << std::endl;
}

int main()
{
    std::atomic_bool end(false);

    // New queue with string type and specified size
    auto myQueue = std::make_shared<customQueue<std::string>>(queueCapacity);

    // Reader and writer threads
    std::thread readerThread(&reader<std::string>, myQueue, &end);
    std::thread writerThread(&writer<std::string>, myQueue, &end);
    
    // Joining threads
    writerThread.join();
    readerThread.join();
    
    return 0;
}
