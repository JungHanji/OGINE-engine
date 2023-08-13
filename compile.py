import os
files = input("Файлы для компиляции (main-файл первый): ").split()
os.system("cls")
print(f"> g++ -c {files[0]} {' '.join(files[1:])} glad.c -I src/include")
os.system(f"g++ -c {files[0]} {' '.join(files[1:])} glad.c -I src/include")
print(f"> g++ -o {files[0][:files[0].index('.')]} {' '.join([fl[:fl.index('.')]+'.o' for fl in files])} glad.o -L src/lib -l sfml-graphics-d -l sfml-window-d -l sfml-network-d -l sfml-system-d -l lua54 -l lua54 -l winmm -l opengl32 -l glu32 -l gdi32 -Wall")
os.system(f"g++ -o {files[0][:files[0].index('.')]} {' '.join([fl[:fl.index('.')]+'.o' for fl in files])} glad.o -L src/lib -l sfml-graphics-d -l sfml-window-d -l sfml-network-d -l sfml-system-d -l lua54 -l lua54 -l winmm -l opengl32 -l glu32 -l gdi32 -Wall")
print(f"> {files[0][:files[0].index('.')]}")
os.system(f"{files[0][:files[0].index('.')]}")