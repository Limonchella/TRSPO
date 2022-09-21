import threading
import time

def timePause1():
    time.sleep(2)

def timePause2():
    time.sleep(3)

def timePause3():
    time.sleep(5)

start = time.time()

thread1 = threading.Thread(target=timePause1, args = ())
thread2 = threading.Thread(target=timePause2, args = ())
thread3 = threading.Thread(target=timePause3, args = ())

thread1.start()
thread2.start()
thread3.start()

thread1.join()
thread2.join()
thread3.join()

print(time.time() - start)
