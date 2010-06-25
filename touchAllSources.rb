require 'fileutils'  
Dir.glob('**/*.cpp').each do |file|  
begin  
    print file + "\n" if !File.directory?(file) && !(file.include? "ext")
	File.utime(Time.new, Time.new, file)
# rescue # this can be dangereous unless used cautiously  
    rescue Errno::ENOTEMPTY  
end  
end  
