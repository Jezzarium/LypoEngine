//
// Created by Justin Abourjaili-Bilodeau on 7/20/2024.
//
#include <core/console_log.h>
#include "core/window.h"
#import "core/rendering/IndexBuffer.hpp"


//#include <Foundation/Foundation.hpp>
//#include <Metal/Metal.hpp>
//#include <QuartzCore/QuartzCore.hpp>

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#include <Metal/Metal.hpp>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/CAMetalLayer.h>

//for triangle
#include <simd/simd.h>

int main()
{
    client_logger.info("MacOS main.cpp test file");
    auto window = hive::Window::create("MacOS Metal Window", 600, 700, hive::WindowFlags::DEFAULT);

    GLFWwindow* glfwWindow = reinterpret_cast<GLFWwindow*>(window->getNativeWindow());
    NSWindow* metalWindow;
    CAMetalLayer* metalLayer;
    MTL::Device*metalDevice = MTL::CreateSystemDefaultDevice();

    metalWindow = glfwGetCocoaWindow(glfwWindow);
    metalLayer = [CAMetalLayer layer];
    metalLayer.device = (__bridge id<MTLDevice>)metalDevice;
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalWindow.contentView.layer = metalLayer;
    metalWindow.contentView.wantsLayer = YES;



    //the actual triangle work
    CA::MetalDrawable* metalDrawable;

    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* triangleVertexBuffer;

    //make the actual triangle
    simd::float3 triangleVertices[] = {
            {-0.5f, -0.5f, 0.0f},
            { 0.5f, -0.5f, 0.0f},
            { 0.0f,  0.5f, 0.0f}
    };

    triangleVertexBuffer = metalDevice->newBuffer(&triangleVertices,
                                                  sizeof(triangleVertices),
                                                  MTL::ResourceStorageModeShared);

    const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };

        v2f vertex vertexMain( uint vertexId [[vertex_id]],
                               device const float3* positions [[buffer(0)]],
                               device const float3* colors [[buffer(1)]] )
        {
            v2f o;
            o.position = float4( positions[ vertexId ], 1.0 );
            o.color = half3 ( colors[ vertexId ] );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";

    using NS::StringEncoding::UTF8StringEncoding;
    //for library (?)
    NS::Error* pError = nullptr;
    metalDefaultLibrary = metalDevice->newLibrary( NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError );
    if(!metalDefaultLibrary){
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }

    //command quueue
    metalCommandQueue = metalDevice->newCommandQueue();

    //render pipeline
    MTL::Function* vertexShader = metalDefaultLibrary->newFunction(NS::String::string("vertexMain", NS::ASCIIStringEncoding));
    assert(vertexShader);
    MTL::Function* fragmentShader = metalDefaultLibrary->newFunction(NS::String::string("fragmentMain", NS::ASCIIStringEncoding));
    assert(fragmentShader);

    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    renderPipelineDescriptor->setLabel(NS::String::string("Triangle Rendering Pipeline", NS::ASCIIStringEncoding));
    renderPipelineDescriptor->setVertexFunction(vertexShader);
    renderPipelineDescriptor->setFragmentFunction(fragmentShader);
    assert(renderPipelineDescriptor);
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)metalLayer.pixelFormat;
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);

    NS::Error* error;
    metalRenderPSO = metalDevice->newRenderPipelineState(renderPipelineDescriptor, &error);

    renderPipelineDescriptor->release();








    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(window->getNativeWindow())))
    {
        /* Render here */
        @autoreleasepool {
                metalDrawable = (__bridge CA::MetalDrawable*)[metalLayer nextDrawable];
                metalCommandBuffer = metalCommandQueue->commandBuffer();

                MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
                MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
                cd->setTexture(metalDrawable->texture());
                cd->setLoadAction(MTL::LoadActionClear);
                cd->setClearColor(MTL::ClearColor(41.0f/255.0f, 42.0f/255.0f, 48.0f/255.0f, 1.0));
                cd->setStoreAction(MTL::StoreActionStore);

//            uint32_t indices[] = { 0, 1, 2 }; // Index order to form a triangle
//            MTL::Buffer* indexBuffer = metalDevice->newBuffer(indices, sizeof(indices), MTL::ResourceStorageModeShared);
//            auto indexBuffer = Lypo::IndexBuffer::create(indices, 3);

                MTL::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
                renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
                renderCommandEncoder->setVertexBuffer(triangleVertexBuffer, 0, 0);
                MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
                NS::UInteger vertexStart = 0;
                NS::UInteger vertexCount = 3;
                renderCommandEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, vertexStart, vertexCount);

                renderCommandEncoder->endEncoding();

                metalCommandBuffer->presentDrawable(metalDrawable);
                metalCommandBuffer->commit();
                metalCommandBuffer->waitUntilCompleted();

                renderPassDescriptor->release();
        }

        /* Poll for and process events */
        window->onUpdate();
    }
    return 0;
}