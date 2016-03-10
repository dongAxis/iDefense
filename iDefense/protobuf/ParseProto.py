import sys
import os
import re
from array import *

class Type:
    def __init__(self ,class_name=None, member_array=[]):
        # self.namespace = namespace
        self.class_name = class_name
        self.member_array = member_array

class ParseProto:
    def __init__(self, inputdir="./", outputdir="./"):
        self.input_dir = os.path.abspath(inputdir)
        if self.input_dir[-1]!='/':
            self.input_dir+='/'

        self.output_dir = os.path.abspath(outputdir)
        if(self.output_dir[-1]!='/'):
            self.output_dir+='/'
        if not os.path.exists(self.output_dir):
            os.makedirs(self.output_dir)

        self.proto_file_list = None
        self.class_array = []
        self.namespace = ""

        self.type_mapper = dict()
        self.type_mapper["int32"]="int32_t"
        self.type_mapper["uint32"]="uint32_t"
        self.type_mapper["string"]="std::string"

    def GetAllProtoFile(self):
        if os.path.exists(self.input_dir) is False:
            raise "file %s dose not existed" % self.input_dir

        tmp_files = os.listdir(self.input_dir)
        self.proto_file_list = []
        for path in tmp_files:
            if path[0]==".":
                continue

            if path[-6:]!=".proto":
                continue

            # if not os.path.isabs(path):
            #     path = self.input_dir+path

            self.proto_file_list.append(path)

        # for path in self.proto_file_list:
        #     print path

    def ParseClassName(self, message):
        pattern = re.compile(r'message\s?[\s|\w]*')
        match = pattern.findall(message)

        if match:
            class_name = match[0];
            split_str = class_name.split(' ')

            if len(split_str)<2:
                return ""

            for i in range(0, len(split_str)):
                if split_str[i].strip()=="message":
                    if i+1 < len(split_str):
                        return split_str[i+1].strip()

        return ""

    def ParseType(self, message):
        pattern = re.compile(r'\{[\w\s\[\]\=\;]*\}')
        match = pattern.findall(message)
        members = []

        if match:
            str = match[0]
            str = str.strip()
            if str[0]=='{':
                str = str[1:]

            if str[-1]=='}':
                str = str[:-2]

            str = str.strip()   #strip r'\s'
            member_array = str.split(';')

            #walk through all the member
            for type in member_array:
                if type == "":
                    continue

                token = type.split()
                if len(token)<3:
                    continue

                tmp_array = []
                tmp_array.append(token[1])      #type
                tmp_array.append(token[2])      #member
                members.append(tmp_array)    #add member to the array

        return members


    def ParseMember(self, message, filename):
        class_name = self.ParseClassName(message)
        if class_name==" ":
            return

        types = self.ParseType(message) #parse type

        return Type(class_name, types)

    def ParseMessage(self, file_content, filename):

        if len(file_content)==0:
            return

        self.namespace=""

        #parse namespace
        pattern = re.compile(r'package[\s\w]+;')
        match = pattern.findall(file_content)

        print match
        if match:
            package = match[0]
            buffer = package.split(' ')
            for index in range(0,len(buffer)):
                if buffer[index]=="package":
                    if index+1<len(buffer):
                        self.namespace = buffer[index+1]
                        self.namespace = self.namespace.strip()
                        if self.namespace[-1]==';':
                            self.namespace = self.namespace[:-1]
                        # print self.namespace
        else:
            self.namespace = ""

        # print self.namespace

        pattern = re.compile(r'message[\s\w\,\[\]\{\=\;]+\}')
        match = pattern.findall(file_content)

        if match:
            for message in match:
                type = self.ParseMember(message, filename)
                self.class_array.append(type)

                #generate file here ?
            self.GenerateHeaderFile(filename)

    def GenerateConvertFromPbToDir(self, fd):
        for type in self.class_array:
            function_declare_header = "bool convert("+type.class_name+" lhs, CFMutableDictionaryRef& rhs) {\n"
            function_declare_tail = "}"

            fd.write(function_declare_header)

            fd.write("\treturn (rhs!=NULL &&\n\t\t\t")

            length = len(type.member_array)
            index = 0
            for member in type.member_array:
                index+=1
                type = self.type_mapper[member[0]]
                mem = member[1]
                print mem
                fd.write("(!lhs.has_"+mem+"() || ")
                fd.write("conver_from_pb_to_simplecf("+mem+"))")
                if index != length:
                    fd.write("&&")
                fd.write("\n\t\t\t")

            fd.write(");\n")
            fd.write(function_declare_tail)
            fd.write("\n\n\n")

    def GenreateConvertFromDirToPb(self, fd):
        for type in self.class_array:
            function_declare_header = "bool convert(CFDictionaryRef lhs, "+type.class_name+"& rhs) {\n"
            function_declare_tail = "\treturn rhs.IsInitialized();\n}"

            fd.write(function_declare_header)
            # fd.write("\tif(rhs==NULL) return false;\n\n")

            for member in type.member_array:
                type = self.type_mapper[member[0]]
                mem = member[1]

                fd.write("\tconvert_from_simplecf_to_pb("+mem+", "+type+");\n")

            fd.write(function_declare_tail)

    def GenerateHeaderFile(self, filename):

        if filename[-2:]!=".h":
            filename+=".h"

        abs_path = self.output_dir+filename

        if os.path.exists(filename):
            raise "file %s is existed, please check it" % filename

        #os.mknod(filename, 0x777)       #create file

        fd = open(filename, "w+")
        if fd==None:
            raise "can not open the file "+ filename

        header = "" \
                 "#ifndef "+filename[:-2].upper()+"_H\n" \
                 "#define "+filename[:-2].upper()+"_H\n" \
                 "#include \"iDFTypeConvert.h\"\n"
        if self.namespace!="":
            header+= "#include \""+filename[:-2]+".pb.h\"\n\n"
            header+= "namespace "+self.namespace.lower()+" { \n\n"

        # print header#self.namespace.upper() + "  " + filename

        tail = "\n\n#endif /*"+filename.upper()+"_h*/"

        fd.write(header)

        #//todo:
        self.GenerateConvertFromPbToDir(fd)
        self.GenreateConvertFromDirToPb(fd)

        if self.namespace!="":
            fd.write("\n\n\n}")
        fd.write(tail)
        fd.close()
        pass

    def GenerateCode(self):

        for filename in self.proto_file_list:
            abs_path = self.input_dir+filename

            if not os.path.exists(abs_path):
                raise "the file %s dose not existed" % abs_path

            fd = open(abs_path, "r")
            file_content = fd.read()    #read whole file

            self.class_array=[]
            self.ParseMessage(file_content, filename.split(".")[0])

    def Run(self):
        try:
            self.GetAllProtoFile()    #get all the protobuf file
            self.GenerateCode()
        except Exception, e:
            print e
            exit(-1)


def main(argv):
    if len(argv)==1:        #no param
        proto = ParseProto()
    elif len(argv)==2:      #have input dir
        proto = ParseProto(argv[1])
    elif len(argv)==3:      #have two param
        proto = ParseProto(argv[1], argv[2])
    else:
        print("error")
        exit(0)

    proto.Run()


# need two input dir and output dir
if __name__=="__main__":
    main(sys.argv)
