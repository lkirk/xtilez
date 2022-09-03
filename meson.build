project('xtilez', 'c', default_options: ['c_std=c99'])

version = run_command('git', 'describe', '--always', '--dirty', check: false).stdout().strip()

c_args = [
  '-Wall',
  '-Wextra',
  '-Wpedantic',
  '-DVERSION="@0@"'.format(version)
]

sources = [
  'client.c',
  'debug.c',
  'exec.c',
  'handlers.c',
  'log.c',
  'main.c',
  'tiling_algorithms.c'
]

executable('xtilez', 'main.c', dependencies: [dependency('x11')], c_args: c_args, sources: sources)
