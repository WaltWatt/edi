project(
    'edi',
    'cpp',
    version :'0.0.1',
    license :'MIT',
    default_options : [
        'cpp_std=c++14',
        'warning_level=2',
        'werror=true',
    ],
)

executable(
    'edi',
    'src/main.cc',
    'src/tui.cc',
    'src/uimode.cc',
    #cpp_args : 'O3',
)

# test
testdir = 'test/'
tests = [
    'test_funct',
]

foreach t : tests
    _test = executable(
        t,
        ['test/main.cc', testdir + t + '.cc'],
    )
    test(t, _test)
endforeach
