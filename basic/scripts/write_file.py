import os
import time


def main():
    print(f'main() enter, cwd: {os.getcwd()}')
    root = os.path.dirname(os.getcwd()) + "/testdata"
    print(f'test root is: {root}')
    for i in range(10 * 100000):
        with open(os.path.join(root, "write_test.txt"), "w+t") as f:
            f.write(f"timer {i}\n")
            pass
        time.sleep(1.5)
        pass


if __name__ == '__main__':
    main()
