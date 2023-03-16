# OS

## TODO  
- Trouver un nom d'os  
- Printf
- Fixer l'assembleur qui initialise la GDT
- Paging
- faire un os https://wiki.osdev.org/Creating_an_Operating_System
- Faire un bootloader pour fuir Grub  https://wiki.osdev.org/Rolling_Your_Own_Bootloader (eventuellement)




## Discussions d'implémentation  
- La segmentation c'est un bordel qui va nous empécher de faire ce qu'on veut avec GCC donc on oublie

- Paging manager : Stack + Bitmap : free / alloc : O(1) (par la stack), Check situation O(1) (par la bitmap)
