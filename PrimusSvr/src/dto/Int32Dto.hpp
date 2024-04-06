#ifndef COUNTDTO_HPP
#define COUNTDTO_HPP

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace primus
{
    namespace dto
    {

#include OATPP_CODEGEN_BEGIN(DTO)

        /**
         * @brief DTO class representing single value.
         */
        class UInt32Dto : public oatpp::DTO
        {

            DTO_INIT(UInt32Dto, DTO);

            DTO_FIELD_INFO(value) {
                info->description = "General value of a single unsigned int";
            }
            DTO_FIELD(oatpp::UInt32, value);

        };

        /**
         * @brief DTO class representing single value.
         */
        class Int32Dto : public oatpp::DTO
        {

            DTO_INIT(Int32Dto, DTO);

            DTO_FIELD_INFO(value) {
                info->description = "General value of a single signed int";
            }
            DTO_FIELD(oatpp::Int32, value);

        };

#include OATPP_CODEGEN_END(DTO)

    } // namespace dto
} // namespace primus

#endif // COUNTDTO_HPP