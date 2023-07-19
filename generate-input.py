import random

def generate_random_integers(file_name, n):
    with open(file_name, 'w') as file:
        for _ in range(n):
            random_integer = random.randint(1, 100)
            sign = random.randint(1, 2)
            if sign == 1:
                random_integer *= -1
            file.write(str(random_integer) + '\n')

# Example usage
generate_random_integers('cmake-build-debug/input.txt', 10)
