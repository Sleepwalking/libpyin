libpyin
===

Probabilistic YIN implementation in C for fundamental frequency extraction.

Compile
---

```
git submodule init
git submodule update
cd external/libgvps
mkdir build
make
cd ../..
mkdir build
make
```

Works cited
---

* Mauch, Matthias, and Sam Dixon. "pYIN: A fundamental frequency estimator using probabilistic threshold distributions." Acoustics, Speech and Signal Processing (ICASSP), 2014 IEEE International Conference on. IEEE, 2014.
* De Cheveigné, Alain, and Hideki Kawahara. "YIN, a fundamental frequency estimator for speech and music." The Journal of the Acoustical Society of America 111.4 (2002): 1917-1930.
* test/vaiueo2d.wav and external/matlabfunctions.c are extracted from [World by M. Morise](http://ml.cs.yamanashi.ac.jp/world/).
