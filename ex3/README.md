# Exercise 3 - Assignment 2

```java
class Flaky implements Lock {
    private int turn;
    private boolean busy = false;
    public void lock () {
        int me = ThreadID.get();
        do {
            do {
                turn = me;
            } while (busy);
        busy = true;
        } while (turn != me);
    }
    public void unlock() {
        busy = false ;
    }
}
```
For each question, either sketch a proof or display an execution where it fails.
1. Does this protocol satisfy mutual exclusion?
2. Is this protocol starvation-free?
3. Is this protocol deadlock-free?
## Does this protocol satisfy mutual exclusion?
The protocol is able to satisfy mutual exclusion. Let's refer to the example of Alice and Bob(two threads) with their pets and a pond (shared resource) with the exclusive access given by the Flaky lock.
The proof of mutual exclusion is by contradiction.<br>
Let's assume that both Alice and Bob are in the outer `do-while` loop, so they are both grabbed the lock.
Let's assume that Alice is the last one who grabbed the lock.
Reasoning backward:
1. Alice has grabbed the lock, this means that she set the flag `busy = true`
2. If she has set the flag `busy = true`, she must have read, in a prior moment, the flag `busy = false`
3. If she has read `busy = false`, that means that Bob should have released the lock (setting `busy = false`)
4. Contraddiction: from the hypotesis Bob still hold the lock.
## [TODO] Is this protocol starvation-free?
No
## [TODO] Is this protocol deadlock-free?
No

## Comparison Flaky lock vs std::mutex
In order to check the behaviour of this lock compared with the one provided by the STL of C++ we have developed a small program to run with different locks.
From the `ex3` folder `cd flaky/`
- Flaky lock
```
make
./main
```
- std::mutex
```
make std
./main
```