#include <VX/vx.h>
#include <VX/vxu.h>
#include <stdio.h>

int main()
{
  vx_uint16 num;
  vx_uint16 vendor;
  vx_context context;
  vx_char imp[VX_MAX_IMPLEMENTATION_NAME];

  context = vxCreateContext();

	vxQueryContext(context, VX_CONTEXT_VERSION, &num, sizeof(num));
  vxQueryContext(context, VX_CONTEXT_IMPLEMENTATION, imp, sizeof(imp));

	printf("Implementation = %s\n",imp);
	printf("Version = %x\n", num);
  vxReleaseContext(&context);
  return 0;
}
