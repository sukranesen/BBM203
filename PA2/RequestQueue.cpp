#include "RequestQueue.h"
#include <new> // for std::nothrow

RequestQueue::RequestQueue()
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    // start with a small default capacity
    resize(4);
}

RequestQueue::RequestQueue(int initialCapacity)
    : data(nullptr),
      capacity(0),
      front(0),
      rear(0),
      count(0) {
    if (initialCapacity < 1) {
        initialCapacity = 4;
    }
    resize(initialCapacity);
}

RequestQueue::~RequestQueue() {
    delete[] data;
}

bool RequestQueue::isEmpty() const {
    return count == 0;
}

bool RequestQueue::isFull() const {
    return count == capacity;
}

int RequestQueue::size() const {
    return count;
}

int RequestQueue::nextIndex(int idx) const {
    if (capacity == 0) return 0;
    return (idx + 1) % capacity;
}

bool RequestQueue::enqueue(const Request& req) {
    //Implement enqueue function as explained in the PDF.
    //(void)req; // suppress unused warning until implemented

    if (isFull()){
        int newCapacity = (capacity==0) ? 4 : capacity*2;
        if (!resize(newCapacity)) {
            return false;
        }
    }

    data [rear] = req;
    rear = nextIndex(rear);
    ++count;
    return true;

}

bool RequestQueue::dequeue(Request& outReq) {
    //Implement dequeue function as explained in the PDF.
    //(void)outReq; // suppress unused warning until implemented
    //return false;
    

    if (isEmpty()) {
        return false;
    }
    outReq = data[front];
    front = nextIndex(front);
    --count;

    if(count ==0){
        front=0;
        rear = 0;
    }
    return true;
}

bool RequestQueue::peek(Request& outReq) const {
    //Implement peek function as explained in the PDF.
    //return true;


    if(isEmpty()){
        return false;
    }
    outReq = data[front];
    return true;
}

void RequestQueue::clear() {
    front = 0;
    rear = 0;
    count = 0;
}

bool RequestQueue::removeById(const std::string& id) {
    //Implement removeById function as explained in the PDF.
    //(void)id; // suppress unused warning until implemented
    //return false;

    if(isEmpty()){
        return false;
    }
    int foundPos = -1;
    int idx = front;
    for (int i = 0; i<count; i++) {
        if (data[idx].getId() == id) {
            foundPos = i;
            break;
        }
        idx = nextIndex(idx);
    }
    if (foundPos == -1) {
        return false;
    }
    for (int i = foundPos; i<count-1; ++i){
        int fromIdx = (front + i +1) % capacity;
        int toIdx = (front + i) % capacity;
        data[toIdx] = data[fromIdx];
    }
    --count;
    if (count == 0) {
        front =0;
        rear=0;
    } else {
        rear = (front + count)%capacity;
    }
    return true;
}

bool RequestQueue::resize(int newCapacity) {
    //Implement resize function as explained in the PDF.
    
    
    if (newCapacity<=0){
        return false;
    }
    Request *newData = new (std::nothrow) Request[newCapacity];
    if (!newData) {
        return false;
    }
    int idx = front;
    for (int i =0; i<count; ++i) {
        newData[i] = data[idx];
        idx = nextIndex(idx);
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
    front = 0;
    rear = count;

    return true;
}
