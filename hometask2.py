import threading
import time
import random
class Value1:
    def __init__(self):
        self.value = 0
    def read(self):
        return self.value
    def increase(self, term):
        self.value += term
class Value2:
    def __init__(self):
        self.value = 0
    def read(self):
        return self.value
    def increase(self, term):
        self.value += term
first_obj = Value1()
second_obj = Value2()
mutex1 = threading.Lock()
mutex2 = threading.Lock()
def increase2(K):
    mutex2.acquire()
    try:
        for i in range(K):
            second_obj.increase(random.randrange(1, 500)/10)
    finally:
        mutex2.release()
    mutex1.acquire()
    try:
        for i in range(K):
            first_obj.increase(random.randrange(1, 500) / 10)
    finally:
        mutex1.release()
def increase1(K):
    mutex1.acquire()
    try:
        for i in range(K):
            first_obj.increase(random.randrange(1, 500)/10)
    finally:
        mutex1.release()
    mutex2.acquire()
    try:
        for i in range(K):
            second_obj.increase(random.randrange(1, 500) / 10)
    finally:
        mutex2.release()

count = random.randint(10,20)
threads = []
for i in range(count//2):
    threads.append(threading.Thread(target = increase1, args = (random.randint(10000, 20000),)))
for i in range(count//2):
    threads.append(threading.Thread(target = increase2, args = (random.randint(10000, 20000),)))
start = time.time()
for thread in threads:
    thread.start()
for thread in threads:
    thread.join()
print(time.time() - start)

