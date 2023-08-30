import os
import time


def main():
    print(f'main() enter, cwd: {os.getcwd()}')
    root = os.path.dirname(os.getcwd()) + "/testdata"
    print(f'test root is: {root}')
    with open(os.path.join(root, "write_test.txt"), "wt") as f:
        for i in range(10):
            f.write(f"timer {i}\n")
            time.sleep(0.25)
        pass
    pass


if __name__ == '__main__':
    main()
