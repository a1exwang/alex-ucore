#!/usr/bin/env ruby
require 'optparse'
require 'pathname'

llvm_bin_path = '/home/alexwang/.CLion2016.1/system/cmake/generated/llvm-v9-master-32433d16/32433d16/Debug/bin'
llc =           File.join llvm_bin_path, 'llc'
lld =           File.join llvm_bin_path, 'lld'
llvm_objdump =  File.join llvm_bin_path, 'llvm-objdump'
llvm_link =     'llvm-link'
clang =         'clang'


clang_flags =    '-c -emit-llvm -fno-builtin -m32 -mno-sse -fno-stack-protector'
lld_flags =      '-flavor gnu'
llc_flags =      '-march=alex -filetype=obj'
llcasm_flags =   '-march=alex -filetype=asm'

suffix = ['c', 'cpp']

VL_DEBUG = 10
VL_VERBOSE = 5
VERBOSE_LEVEL = VL_DEBUG

LOG_IO = STDOUT

def log(s)
  LOG_IO.puts s if VERBOSE_LEVEL >= VL_DEBUG
end
def run_log(cmd)
  output = `#{cmd}`
  if VERBOSE_LEVEL >= VL_VERBOSE
    LOG_IO.write output
  end
end
# option parser start
options = {
  inc_path: [],
  src_dir: File.expand_path('./src'),
  obj_dir: File.expand_path('./obj'),
  output_file: File.expand_path('./bin/a.out')
}
option_parser = OptionParser.new do |opts|
  opts.banner = './build -s src_dir -b obj_dir -o output_file'

  opts.on('-s SRC_DIR', '--source-dir SRC_DIR', 'set source dir') do |s|
    options[:src_dir] = File.expand_path(s)
  end

  opts.on('-b OBJ_DIR', '--obj-dir OBJ_DIR', 'set obj dir') do |s|
    options[:obj_dir] = File.expand_path(s)
  end

  opts.on('-o OUTPUT_FILE', 'set output file') do |s|
    options[:output_file] = File.expand_path(s)
  end

  opts.on('-I INC_PATH', 'include path') do |s|
    options[:inc_path] << File.expand_path(s)
  end

end.parse!
# option parser end

includes = options[:inc_path] + (`find #{options[:src_dir]} -type d`.split "\n")
include_params = includes.reduce('') { |sum, inc| sum + " -I#{inc}" }

# clang compile c to llvm ir
cmd = "find #{options[:src_dir]} | grep -E \"\\.(#{suffix.join '|'})$\""
puts cmd
sources = `#{cmd}`.split "\n"
llvm_objs = []
sources.each do |source|
  rel_path = Pathname.new(File.expand_path source).relative_path_from(Pathname.new options[:src_dir]).to_s
  file_name = rel_path.split('/').last
  llvm_obj_file = File.join(options[:obj_dir], rel_path + '.bc')
  llvm_objs << llvm_obj_file
  run_log "mkdir -p #{llvm_obj_file.split('/')[0...-1].join('/')}"
  # .c => .bc
  run_log "clang #{clang_flags} #{include_params} #{source} -o #{llvm_obj_file}"
  # .bc => .ll, for debug
  run_log "#{llc} #{llcasm_flags} #{llvm_obj_file} -o #{llvm_obj_file}.ll"
end

run_log "mkdir -p #{options[:output_file].split('/')[0...-1].join('/')}"
llvm_one_big_obj = options[:output_file] + '.bc'
alex_obj = options[:output_file] + '.o'

# link to one llvm ir file
run_log "llvm-link -o #{llvm_one_big_obj} #{llvm_objs.join ' '}"

# compile llvm ir to alex-machine obj file
run_log "#{llc} #{llc_flags} #{llvm_one_big_obj} -o #{alex_obj}"
run_log "#{llc} #{llcasm_flags} #{llvm_one_big_obj} -o #{alex_obj}.S"

# link
run_log "#{lld} #{lld_flags} #{alex_obj} -o #{options[:output_file]}"
