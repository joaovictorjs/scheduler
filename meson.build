project('scheduler', 'cpp', version: '0.0.1')

include = include_directories('include')

static = static_library(
	meson.project_name(),
	install: true,
	sources: 'src/scheduler.cpp',
	include_directories: include,
)

shared = shared_library(
	meson.project_name(),
	version: meson.project_version(),
	install: true,
	sources: 'src/scheduler.cpp',
	include_directories: include,
)

install_headers('include/scheduler.hpp', subdir: meson.project_name())

pkg = import('pkgconfig')

pkg.generate(
	name: meson.project_name(),
	description: 'A cpp library for scheduling tasks',
	version: meson.project_version(),
	subdirs: meson.project_name(), 
	libraries: [static, shared]
)

subdir('test')
