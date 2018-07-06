#include <VX/vx.h>
#include <VX/vxu.h>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <stdio.h>

#define ERROR_CHECK_STATUS( status ) { \
        vx_status status_ = (status); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }

#define ERROR_CHECK_OBJECT( obj ) { \
        vx_status status_ = vxGetStatus((vx_reference)(obj)); \
        if(status_ != VX_SUCCESS) { \
            printf("ERROR: failed with status = (%d) at " __FILE__ "#%d\n", status_, __LINE__); \
            exit(1); \
        } \
    }

void copy_to_vx(const cv::Mat cv_buf, vx_image vx_buf); 
void copy_to_cv(const vx_image vx_buf, cv::Mat cv_buf);   

int main()
{
   vx_context context = NULL;
   vx_graph graph = NULL;
   vx_image vx_in_image = NULL;
   vx_image vx_out_image = NULL;
   cv::Mat cv_in_image;
   cv::Mat cv_out_image;
   cv::Mat cv_gray;
   cv::Size image_size = cv::Size(640, 480);
   vx_threshold thrs = NULL;
   vx_int32 low_thrs = 1;
   vx_int32 high_thrs = low_thrs * 3;
   
   
   context = vxCreateContext();
   ERROR_CHECK_OBJECT(context);
   
   graph = vxCreateGraph(context);
   ERROR_CHECK_OBJECT(graph);
   
   
   cv_in_image = cv::imread("messi5.jpg");
   if(cv_in_image.empty()){
     printf("Error load image.\n");
     exit(1);
   }
   
   if(cv_in_image.size() != image_size){
     resize(cv_in_image, cv_in_image, image_size);
   }  
   cv::cvtColor(cv_in_image, cv_gray, cv::COLOR_BGR2GRAY);
   cv_out_image.create(image_size, CV_8UC1);
      
   vx_in_image = vxCreateImage(context, image_size.width, image_size.height, VX_DF_IMAGE_U8);
   ERROR_CHECK_OBJECT(vx_in_image);
   vx_out_image = vxCreateImage(context,  image_size.width, image_size.height, VX_DF_IMAGE_U8);
   ERROR_CHECK_OBJECT(vx_out_image);
   
   copy_to_vx(cv_gray, vx_in_image);
   thrs = vxCreateThreshold(context, VX_THRESHOLD_TYPE_RANGE, VX_TYPE_UINT8);
   ERROR_CHECK_OBJECT(thrs);
   ERROR_CHECK_STATUS(vxSetThresholdAttribute(thrs, VX_THRESHOLD_THRESHOLD_LOWER, &low_thrs, sizeof(low_thrs)));
   ERROR_CHECK_STATUS(vxSetThresholdAttribute(thrs, VX_THRESHOLD_THRESHOLD_UPPER, &high_thrs, sizeof(high_thrs)));
   
   ERROR_CHECK_OBJECT(vxCannyEdgeDetectorNode(graph, vx_in_image, thrs, 3, VX_NORM_L1, vx_out_image));
   copy_to_cv(vx_out_image, cv_out_image);
   
   ERROR_CHECK_STATUS(vxVerifyGraph(graph));
   ERROR_CHECK_STATUS(vxProcessGraph(graph));
   
   copy_to_cv(vx_out_image, cv_out_image);
   
   imshow("vxProcessedImage", cv_out_image);
      
   cv_out_image = cv::Scalar::all(0);
   cv::Canny(cv_gray, cv_out_image, low_thrs, high_thrs, 3);
   
   imshow("cvProcessedImage", cv_out_image);
   
   while(cv::waitKey(33) != 'q');
   
   cv::imwrite("canny.jpg", cv_out_image);
       
   ERROR_CHECK_STATUS(vxReleaseGraph(&graph));
   ERROR_CHECK_STATUS(vxReleaseImage(&vx_in_image));
   ERROR_CHECK_STATUS(vxReleaseImage(&vx_out_image));
   ERROR_CHECK_STATUS(vxReleaseThreshold(&thrs));
   ERROR_CHECK_STATUS(vxReleaseContext(&context));
   
   return 0;
}

void copy_to_vx(const cv::Mat cv_buf, vx_image vx_buf){
  const vx_rectangle_t rect = {0, 0, static_cast<vx_uint32>(cv_buf.cols), static_cast<vx_uint32>(cv_buf.rows)};
  const vx_imagepatch_addressing_t image_info = {static_cast<vx_uint32>(cv_buf.cols), static_cast<vx_uint32>(cv_buf.rows), static_cast<vx_int32>(cv_buf.elemSize()), cv_buf.cols};
  
  vx_uint8 *data_ptr = cv_buf.data;
  ERROR_CHECK_STATUS(vxCopyImagePatch(vx_buf, &rect, 0u, &image_info, data_ptr, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST));  
}

void copy_to_cv(const vx_image vx_buf, cv::Mat cv_buf){
  const vx_rectangle_t rect = {0, 0, static_cast<vx_uint32>(cv_buf.cols), static_cast<vx_uint32>(cv_buf.rows)};
  const vx_imagepatch_addressing_t image_info = {static_cast<vx_uint32>(cv_buf.cols), static_cast<vx_uint32>(cv_buf.rows), static_cast<vx_int32>(cv_buf.elemSize()), cv_buf.cols};
  vx_uint8 *data_ptr = cv_buf.data;
  ERROR_CHECK_STATUS(vxCopyImagePatch(vx_buf, &rect, 0u, &image_info, data_ptr, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
}
