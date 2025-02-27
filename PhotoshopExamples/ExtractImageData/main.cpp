/*
Example of loading a PhotoshopFile and extracting the image data, this can be freely used with any other operations present on the LayeredFile
*/

#include "PhotoshopAPI.h"

#include <unordered_map>
#include <vector>


int main()
{
	using namespace NAMESPACE_PSAPI;

	// In this case we already know the bit depth but otherwise one could use the PhotoshopFile.m_Header.m_Depth
	// variable on the PhotoshopFile to figure it out programmatically. This would need to be done using the 
	// "extended" read signature shown in the ExtendedSignature example.
	LayeredFile<bpp8_t> layered_file = LayeredFile<bpp8_t>::read("ImageData.psb");

	// We could also use findLayer() on the LayeredFile but this way we directly get the appropriate type
	auto img_layer_ptr = find_layer_as<bpp8_t, ImageLayer>("RedLayer", layered_file);

	// We can now either extract just the channels we want:
	std::vector<bpp8_t> channel_r = img_layer_ptr->get_channel(Enum::ChannelID::Red);
	std::vector<bpp8_t> channel_g = img_layer_ptr->get_channel(Enum::ChannelID::Green);
	std::vector<bpp8_t> channel_b = img_layer_ptr->get_channel(Enum::ChannelID::Blue);

	// or extract all the channels as one unordered_map:
	auto channels = img_layer_ptr->get_image_data();

	// over which we could loop like this:
	for (auto& [key, value] : channels)
	{
		// key is a Enum::ChannelIDInfo object where the .id is a Enum::ChannelID 
		// and .index is an unsigned integer

		// value is simply a vector of type T, in our case a std::vector<bpp8_t>

		// This will also extract the channel mask if it exists
	}

	// If we want to extract e.g. the layer mask:
	auto mask_img_layer_ptr = find_layer_as<bpp8_t, ImageLayer>("Group/EmptyLayerWithMask", layered_file);

	// If this doesnt have a mask channel we will simply get an empty channel. In this case though, even though
	// we have a mask it will be empty as well as Photoshop fills in the gaps in the layer with the mask_default_color
	// parameter.
	std::vector<bpp8_t> channel_mask = mask_img_layer_ptr->get_mask();

	// To extract this default color we can do this:
	if (mask_img_layer_ptr->has_mask())
	{
		// This value is always uint8_t even for 16- and 32- bit files!
		uint8_t default_color = mask_img_layer_ptr->mask_default_color();
	}
	// This would tell us that we have an empty white layer mask with no pixel values.
	// One can however write out explicit zeroes for mask channels or set a default color
}