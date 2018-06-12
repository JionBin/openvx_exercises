# Install OpenVX1.1 with OpenCL on Ubuntu16.04

<img src="./OpenVX.png"  height="100px"  style="float:right">

OpenVX can be installed step by step following [README.md](https://www.khronos.org/registry/OpenVX/) in OpenVX Sample, but there are some errors on my side if I enable OpenCL. Here are some steps of installing and modifications.


## 1.  Modifications

> diff --git a/CMakeLists.txt b/CMakeLists.txt          
> index 17e6fac..374d736 100644                       
> --- a/CMakeLists.txt                  
> +++ b/CMakeLists.txt                                         
> @@ -171,6 +171,6 @@ add_subdirectory( helper )                                
 add_subdirectory( kernels )                              
 add_subdirectory( libraries )                             
 add_subdirectory( sample )                            
-add_subdirectory( sample-c++ )                                      
+#add_subdirectory( sample-c++ )                                    
 add_subdirectory( tools )                           
 add_subdirectory( include )              
diff --git a/sample/targets/opencl/vx_interface.c b/sample/targets/opencl/vx_interface.c              
index 70bfde9..e0c107d 100644              
--- a/sample/targets/opencl/vx_interface.c              
+++ b/sample/targets/opencl/vx_interface.c              
@@ -43,7 +43,7 @@              
 static const vx_char name[VX_MAX_TARGET_NAME] = "pc.opencl";               
 /*! \brief Prototype for assigning to kernel */              
-static vx_status vxclCallOpenCLKernel(vx_node node, const vx_reference *parameters, vx_uint32 num);              
+static vx_status vxclCallOpenCLKernel(vx_node node, vx_reference parameters[], vx_uint32 num);               
 static vx_cl_kernel_description_t *cl_kernels[] =              
 {
@@ -323,6 +323,7 @@ vx_status vxTargetInit(vx_target_t *target)              
                                     cl_kernels[k]->description.name,                                          
                                     cl_kernels[k]->description.parameters,              
                                     cl_kernels[k]->description.numParams,              
                                     + cl_kernels[k]->description.validate,              
                                     cl_kernels[k]->description.input_validate,              
                                     cl_kernels[k]->description.output_validate,              
                                     cl_kernels[k]->description.initialize,              
@@ -458,6 +459,7 @@ vx_kernel vxTargetAddKernel(vx_target_t *target,              
                             vx_enum enumeration,              
                             vx_kernel_f func_ptr,              
                             vx_uint32 numParams,              
                             +                            vx_kernel_input_validate_f validate,              
                             vx_kernel_input_validate_f input,              
                             vx_kernel_output_validate_f output,              
                             vx_kernel_initialize_f initialize,              
@@ -474,7 +476,7 @@ vx_kernel vxTargetAddKernel(vx_target_t *target,              
                                kernel,              
                                enumeration, func_ptr, name,              
                                NULL, numParams,              
                                -                               input, output, initialize, deinitialize);              
                                +                               validate, input, output, initialize, deinitialize);              
             VX_PRINT(VX_ZONE_KERNEL, "Reserving %s Kernel[%u] for %s\n", target->name, k, kernel->name);              
             target->num_kernels++;              
             break;              
@@ -515,7 +517,7 @@ vx_cl_kernel_description_t *vxclFindKernel(vx_enum enumeration)               
-vx_status vxclCallOpenCLKernel(vx_node node, vx_reference parameters[], vx_uint32 num)              
+static vx_status vxclCallOpenCLKernel(vx_node node, vx_reference parameters[], vx_uint32 num)              
 {              
     vx_status status = VX_FAILURE;              
     vx_context context = node->base.context;              

## Install OpenVX on Ubuntu

    $: cd ~/openvx_sample
    $: python Build.py --os=Linux --opencl=OpenCL

## Test
    $: export LD_LIBRARY_PATH=install/Linux/x64/Release/bin
    $: cd raw/
    $: ../install/Linux/x64/Release/bin/vx_test

## Log:

> [VX_TEST] Kernels:45 Modules:1 Refs:73  
> [PASSED][00] Framework: Load XYZ Extension  
> [PASSED][01] Framework: Load Kernel Node 
>  [PASSED][02] Framework: Copy  
>  [PASSED][03] Framework: Copy Virtual  
>  [PASSED][04] Framework: Heads  
>  [PASSED][05] Framework: Unvisited  
>  [PASSED][06] Framework: Virtual Image  
>  [vx_test_framework_delay_graph:860] Passed! 
>   [PASSED][07] Framework: Delay  
>   [PASSED][08] Framework: Kernels 
>    [PASSED][09] Direct: Copy Image  
>    [PASSED][10] Direct: Copy External Image  
>    [PASSED][11] Graph: ColorBars YUV  Found 0 differences between images (status=0)  
>    [PASSED][12] Graph: ColorBars RGB  
>    Min Value in AbsDiff = 0, at 285,40  
>    Max Value in AbsDiff = 81, at 32,32  
>    AbsDiff Mean = 43.941765  
>    AbsDiff Stddev = 23.627544  
>    histogram[0] = 2623 
>    histogram[1] = 2589  
>    histogram[2] = 2235  
>    histogram[3] = 2371  
>    histogram[4] = 2805  
>    histogram[5] = 2685  
>    histogram[6] = 2849  
>    histogram[7] = 2938  
>    histogram[8] = 2988  
>    histogram[9] = 4863  
>    histogram[10] = 4142 
>    histogram[11] = 3359  
>    histogram[12] = 3744  
>    histogram[13] = 3776  
>    histogram[14] = 3701  
>    histogram[15] = 12332  
>    [PASSED][13] Graph: bikegray  
>    Nodes[0] average exec: 0.013ms  
>    Nodes[1] average exec: 6.890ms  
>    Nodes[2] average exec: 0.957ms  
>    Nodes[3] average exec: 0.968ms  
>    Nodes[4] average exec: 1.356ms  
>    Nodes[5] average exec: 1.383ms  
>    Nodes[6] average exec: 0.228ms  
>    Nodes[7] average exec: 0.346ms  
>    Nodes[8] average exec: 3.693ms  
>    Nodes[9] average exec: 0.339ms  
>    Nodes[10] average exec: 0.289ms  
>    Nodes[11] average exec: 0.026ms  
>    Nodes[12] average exec: 0.027ms  
>    Nodes[13] average exec: 0.029ms  
>    Nodes[14] average exec: 0.051ms  
>    Nodes[15] average exec: 0.050ms  
>    Nodes[16] average exec: 0.048ms  
>    Nodes[17] average exec: 0.028ms  
>    Nodes[18] average exec: 0.024ms  
>    Nodes[19] average exec: 0.030ms  
>    Graph average exec: 16.776ms  
>    [PASSED][14] Graph: Lena  Multiple writers failed with -23!  
>    [PASSED][15] Graph: Accumulates              
>    **[PASSED][16] Graph: OpenCL**                 
>    [PASSED][17] Graph: Bitwise  
>    [PASSED][18] Graph: Arithmetic  
>    There are 0 number of points in the harris array!  
>    There are 482 number of points in the fast array!  
>    Rectangle from Gaussian is {1, 1, 159, 119}  
>    [PASSED][19] Graph: Corners  
>    [PASSED][20] Graph: Tracker  
>    [PASSED][21] Export: DOT  
>    Passed 22 out of 22
