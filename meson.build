project('ocher', ['cpp', 'c'],
    version: '0.1.0',
    default_options : 'cpp_std=c++11',
    license: 'GPL3')

if get_option('buildtype').startswith('debug')
    add_project_arguments('-DDEBUG', language : 'cpp')
endif

conf_data = configuration_data()
conf_data.set('TOP_SRCDIR', meson.source_root())

thread_dep = dependency('threads')

catch_sp = subproject('catch')
catch_dep = catch_sp.get_variable('catch_dep')

Signals_sp = subproject('Signals')
Signals_dep = Signals_sp.get_variable('Signals_dep')

json_sp = subproject('json')
json_dep = json_sp.get_variable('json_dep')

if get_option('airbag_fd')
    airbag_fd_sp = subproject('airbag_fd')
    airbag_fd_dep = airbag_fd_sp.get_variable('airbag_fd_dep')
else
    airbag_fd_dep = disabler()
endif
if airbag_fd_dep.found()
    conf_data.set('AIRBAG_FD', true)
endif

zlib_dep = dependency('zlib', fallback : ['zlib', 'zlib_dep'])

libev_dep = dependency('libev', fallback : ['libev', 'libev_dep'])

mxml_dep = dependency('mxml', fallback : ['mxml', 'mxml_dep'])

freetype_dep = dependency('freetype2', fallback : ['freetype', 'freetype_dep'])

if get_option('sdl')
    sdl_dep = dependency('sdl')
else
    sdl_dep = dependency('sdl', required : false)
endif
if sdl_dep.found()
    conf_data.set('UX_FB', true)
    conf_data.set('UX_FB_SDL', true)
endif

if get_option('ux_fd')
    conf_data.set('UX_FD', true)
endif

if get_option('ux_cdk')
    cdk_dep = dependency('cdk')
else
    cdk_dep = dependency('cdk', required : false)
endif
if cdk_dep.found()
    conf_data.set('UX_CDK', true)
endif

if get_option('ux_fb')
    conf_data.set('UX_FB', true)
endif

if get_option('epub')
    conf_data.set('FMT_EPUB', true)
endif

if get_option('html')
    conf_data.set('FMT_HTML', true)
endif

if get_option('jpeg')
    conf_data.set('FMT_JPEG', true)
endif

if get_option('png')
    conf_data.set('FMT_PNG', true)
endif

subdir('ocher')
subdir('doc')
