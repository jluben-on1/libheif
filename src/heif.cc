/*
 * HEIF codec.
 * Copyright (c) 2017 struktur AG, Dirk Farin <farin@struktur.de>
 *
 * This file is part of libheif.
 *
 * libheif is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libheif is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libheif.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "heif.h"
#include "heif_image.h"
#include "heif_file.h"
#include "error.h"


#include <memory>

using namespace heif;


struct heif_pixel_image
{
  std::shared_ptr<heif::HeifPixelImage> image;
};



struct heif_context
{
  std::shared_ptr<heif::HeifFile> context;
};


heif_context* heif_context_alloc()
{
  struct heif_context* ctx = new heif_context;
  ctx->context = std::make_shared<HeifFile>();

  return ctx;
}

void heif_context_free(heif_context* ctx)
{
  delete ctx;
}

heif_error heif_context_read_from_file(heif_context* ctx, const char* filename)
{
  Error err = ctx->context->read_from_file(filename);
  return err.error_struct();
}

heif_error heif_context_read_from_memory(heif_context* ctx, const uint8_t* mem, uint64_t size)
{
  Error err = ctx->context->read_from_memory(mem,size);
  return err.error_struct();
}

// TODO
//heif_error heif_context_read_from_file_descriptor(heif_context*, int fd);

// NOTE: data types will change ! (TODO)
heif_error heif_context_get_primary_image(heif_context* ctx, heif_pixel_image** img)
{
  if (!img) {
    // TODO
  }

  uint16_t primary_ID = ctx->context->get_primary_image_ID();
  *img = new heif_pixel_image();

  Error err = ctx->context->decode_image(primary_ID, (*img)->image);
  return err.error_struct();
}


int heif_context_get_number_of_images(heif_context* ctx)
{
  return ctx->context->get_num_images();
}


// NOTE: data types will change ! (TODO)
heif_error heif_get_image(heif_context* ctx, int image_ID, heif_pixel_image** img)
{
  if (!img) {
    // TODO
  }

  Error err = ctx->context->decode_image(image_ID, (*img)->image);
  return err.error_struct();
}




struct heif_pixel_image* heif_pixel_image_create(int width, int height,
                                                 heif_colorspace colorspace,
                                                 heif_chroma chroma)
{
  struct heif_pixel_image* pixel_image = new heif_pixel_image;
  pixel_image->image = std::make_shared<HeifPixelImage>();

  pixel_image->image->create(width, height, colorspace, chroma);

  return pixel_image;
}


void heif_pixel_image_add_plane(struct heif_pixel_image* image,
                                heif_channel channel, int width, int height, int bit_depth)
{
  image->image->add_plane(channel, width, height, bit_depth);
}


const uint8_t* heif_pixel_image_get_plane_readonly(const struct heif_pixel_image* image,
                                                   enum heif_channel channel,
                                                   int* out_stride)
{
  return image->image->get_plane(channel, out_stride);
}


uint8_t* heif_pixel_image_get_plane_readonly(struct heif_pixel_image* image,
                                             enum heif_channel channel,
                                             int* out_stride)
{
  return image->image->get_plane(channel, out_stride);
}


void heif_register_decoder(heif_context* heif, uint32_t type, const heif_decoder_plugin* decoder_plugin)
{
}