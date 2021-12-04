import random

def create_test(n, a, b, filename):
    file = open(filename, 'w')
    delete_order = set()
    for i in range(n):
        value = random.randint(a, b)
        file.write("i " + str(value) + '\n')
        file.write("c\n")
        #file.write("p\n")
        delete_order.add(value)
    delete_order = list(delete_order)
    random.shuffle(delete_order)
    for val in delete_order:
        file.write("d " + str(val) + '\n')
        file.write("c\n")
        #file.write("p\n")
    file.close()

for i in range(100):
    create_test(50, 0, 100, "tests\\test" + str(i) + ".txt")
    
