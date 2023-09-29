exe = executable(
	'scheduler_test', 
	sources: 'scheduler_test.cpp', 
	include_directories: include,
	dependencies: [dependency('gtest'), dependency('gmock')],
	link_with: shared
)

test('scheduler_test', exe)