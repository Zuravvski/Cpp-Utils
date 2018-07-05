#pragma once

#include <type_traits>

#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class As
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

        template <class TBase, class T>
        struct EnforceBaseOf
        {
            static_assert(std::is_base_of< TBase, T >::value && !std::is_same< TBase, T >::value, "TBase should be a base of T");

            static void act() {}
        };

        template <class TBase>
        struct EnforceBaseNotAlreadyRegistered
        {
            static_assert(!TDescriptorInfo::template IsBaseRegistered< TBase >::value, "TBase is already registered for instance T");

            static void act() {}
        };

    public:
        template <class TBase, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterBase< TBase >::Type
        >
        ::Type& as()
        {
            EnforceBaseOf< TBase, InstanceType >::act();
            EnforceBaseNotAlreadyRegistered< TBase >::act();

            auto descriptor = static_cast< TDescriptor* >(this);
            descriptor->addTypeIfMissing(createKeyForType< TBase >(), [](const std::shared_ptr< void >& x)
            {
                auto instanceDynamicType = std::static_pointer_cast< InstanceType >(x);
                auto instanceStaticType = std::static_pointer_cast< TBase >(instanceDynamicType);
                return instanceStaticType;
            });

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterBase< TBase >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

    protected:
        virtual ~As() {}
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic