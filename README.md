# History

## original
The original rtgp is still
[here](https://mathphys.fsk.uni-heidelberg.de/~pseyfert/cc.html)

See the video
[here](http://virgilio.mib.infn.it/~seyfert/dumb-root-terminal.html)
demonstrating what it does.

## more recent

Wrapping all in
[gnuplot-iostream](https://github.com/pseyfert/gnuplot-iostream) seemed a bit
more elegant - although … it also just does a syscall - using `popen` instead
of `system`. And more recently I also saw how to use temporary files
[here](http://www.gnu.org/software/libc/manual/html_node/Temporary-Files.html)
linked from
[here](http://stackoverflow.com/questions/159983/how-can-i-create-a-temporary-file-for-writing-in-c-on-a-linux-platform)

The (in my eyes) greatest disadvantage is that the gnuplot-iostream approach
depends on boost libraries which caused an incredible amount of trouble in all
of my projects (counter intuitive syntax and -worse- breaks whenever moving to
a new computing cluster, compiler version, operating system).

For now the system loads are:
```
gSystem->Load("path/to/libboost_system.so");
gSystem->Load("path/to/libboost_iostreams.so");
gSystem->Load("path/to/librtgp.so")
```

