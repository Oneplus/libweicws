#!/python/env python

from distutils.core import setup,Extension

setup(
    name="weicws",
    description="libweicws python interface",
    long_description="""It's a interface for libweicws,
                        but there are still some bugs to run.""",
    author="Chen Jing",
    url="http://github.com/Oneplus/libweicws/",
    version="0.1",
    ext_modules=[
        Extension("_weicws",
            ["weicws.i","../src/weicws.cpp"],
            swig_opts=["-c++"],
            include_dirs=["../src","../src/utils","../src/internal"],
            library_dirs=["../lib"],
            libraries=["cqdb","crfsuite","lbfgs","pcre","pcrecpp","pcreposix","weicws"])
        ],
    py_modules=["weicws"]
    )
