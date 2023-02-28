#!/usr/bin/python3
import subprocess

def run_program():
    result = subprocess.run(["/home/client/Main/client"], stdout = subprocess.PIPE)
    print(result.stdout.decode('utf-8'))

if __name__ == "__main__":
    run_program()
