#ifndef DATABASE_CLIENT
#define DATABASE_CLIENT

#include "oatpp-sqlite/orm.hpp"
#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/database/MemberDTO.hpp"
#include "dto/database/AdressDTO.hpp"
#include "dto/database/DatabaseDTOs.hpp"
#include "dto/database/DepartmentDTO.hpp"
#include "dto/database/EmailDTO.hpp"
#include "dto/database/MemberDTO.hpp"
#include "dto/database/MembershipDTO.hpp"
#include "dto/database/PhoneNumberDTO.hpp"
#include "dto/database/TrainingDTO.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

/**
 * @brief DatabaseClient class represents a client to interact with the database.
 */
class DatabaseClient : public oatpp::orm::DbClient {
public:

    /**
     * Constructor to initialize the DatabaseClient.
     * @param executor - shared pointer to oatpp executor.
     */
    DatabaseClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
        : oatpp::orm::DbClient(executor)
    {
        oatpp::orm::SchemaMigration migration(executor);
        migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "/001_init.sql");
        // TODO - Add more migrations here.
        migration.migrate(); // <-- run migrations. This guy will throw on error.

        auto version = executor->getSchemaVersion();
        OATPP_LOGD("UserDb", "Migration - OK. Version=%lld.", version);
    }

    //  __  __                _                  ___                            
    // |  \/  | ___ _ __ ___ | |__   ___ _ __   / _ \ _   _  ___ _ __ _   _ ___ 
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__| | | | | | | |/ _ \ '__| | | / __|
    // | |  | |  __/ | | | | | |_) |  __/ |    | |_| | |_| |  __/ |  | |_| \__ \
    // |_|  |_|\___|_| |_| |_|_.__/ \___|_|     \__\_\\__,_|\___|_|   \__, |___/
    //                                                                |___/     

    /**
     * @brief Creates a new member entry in the database.
     *
     * Inserts a new record into the "Member" table.
     *
     * @param member The member to be created as a MemberDTO object.
     */
    QUERY(createMember,
        "INSERT INTO Member "
        "(firstName, lastName, birthDate, joinDate, active) "
        "VALUES (:member.firstName, :member.lastName, :member.birthDate, :member.joinDate, :member.active);",
        PARAM(oatpp::Object<MemberDTO>, member));

    /**
     * @brief Updates an existing member entry in the database.
     *
     * Updates the record in the "Member" table with the provided member ID.
     *
     * @param member The member information to update as a MemberDTO object.
     */
    QUERY(updateMember,
        "UPDATE Member "
        "SET "
        "firstName=:member.firstName, "
        "lastName=:member.lastName, "
        "birthDate=:member.birthDate, "
        "joinDate=:member.joinDate, "
        "active=:member.active "
        "WHERE "
        "id=:member.id;",
        PARAM(oatpp::Object<MemberDTO>, member));

    /**
     * @brief Retrieves a member entry from the database by ID.
     *
     * Retrieves the record from the "Member" table with the specified ID.
     *
     * @param id The ID of the member to retrieve.
     */
    QUERY(getMemberById,
        "SELECT * FROM Member WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves active member entries from the database.
     *
     * Retrieves records from the "Member" table where the 'active' column is set to 1,
     * with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getActiveMembers,
        "SELECT * FROM Member WHERE active=1 LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Retrieves inactive member entries from the database.
     *
     * Retrieves records from the "Member" table where the 'active' column is set to 0,
     * with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getInactiveMembers,
        "SELECT * FROM Member WHERE active=0 LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes a member entry from the database by ID.
     *
     * Deletes the record from the "Member" table with the specified ID.
     *
     * @param id The ID of the member to delete.
     */
    QUERY(deleteMemberById,
        "DELETE FROM Member WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Deactivates a member entry in the database by ID.
     *
     * Sets the 'active' flag to 0 for the member record in the "Member" table with the specified ID.
     *
     * @param id The ID of the member to deactivate.
     */
    QUERY(deactivateMember,
        "UPDATE Member SET active=0 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Activates a member entry in the database by ID.
     *
     * Sets the 'active' flag to 1 for the member record in the "Member" table with the specified ID.
     *
     * @param id The ID of the member to activate.
     */
    QUERY(activateMember,
        "UPDATE Member SET active=1 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    //     _       _                      ___                            
    //    / \   __| |_ __ ___  ___ ___   / _ \ _   _  ___ _ __ _   _ ___ 
    //   / _ \ / _` | '__/ _ \/ __/ __| | | | | | | |/ _ \ '__| | | / __|
    //  / ___ \ (_| | | |  __/\__ \__ \ | |_| | |_| |  __/ |  | |_| \__ \
    // /_/   \_\__,_|_|  \___||___/___/  \__\_\\__,_|\___|_|   \__, |___/
    //                                                         |___/     

    /**
     * @brief Creates a new address entry in the database.
     *
     * Inserts a new record into the "Address" table with the provided address information.
     *
     * @param address The address to be created as an AddressDTO object.
     */
    QUERY(createAddress,
        "INSERT INTO Address (zipCode, city, state, houseNumber, street) "
        "VALUES (:address.zipCode, :address.city, :address.state, :address.houseNumber, :address.street);",
        PARAM(oatpp::Object<AddressDTO>, address));

    /**
     * @brief Updates an existing address entry in the database.
     *
     * Updates the record in the "Address" table with the provided address ID.
     *
     * @param address The address information to update as an AddressDTO object.
     */
    QUERY(updateAddress,
        "UPDATE Address "
        "SET "
        "zipCode=:address.zipCode, "
        "city=:address.city, "
        "state=:address.state, "
        "houseNumber=:address.houseNumber, "
        "street=:address.street "
        "WHERE "
        "id=:address.id;",
        PARAM(oatpp::Object<AddressDTO>, address));

    /**
     * @brief Retrieves an address entry from the database by ID.
     *
     * Retrieves the record from the "Address" table with the specified ID.
     *
     * @param id The ID of the address to retrieve.
     */
    QUERY(getAddressById,
        "SELECT * FROM Address WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all address entries from the database.
     *
     * Retrieves records from the "Address" table with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllAddresses,
        "SELECT * FROM Address LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes an address entry from the database by ID.
     *
     * Deletes the record from the "Address" table with the specified ID.
     *
     * @param id The ID of the address to delete.
     */
    QUERY(deleteAddressById,
        "DELETE FROM Address WHERE id=:id;",
        PARAM(oatpp::Int64, id));


    //  ____  _                      _   _                 _               
    // |  _ \| |__   ___  _ __   ___| \ | |_   _ _ __ ___ | |__   ___ _ __ 
    // | |_) | '_ \ / _ \| '_ \ / _ \  \| | | | | '_ ` _ \| '_ \ / _ \ '__|
    // |  __/| | | | (_) | | | |  __/ |\  | |_| | | | | | | |_) |  __/ |   
    // |_|__ |_| |_|\___/|_| |_|\___|_| \_|\__,_|_| |_| |_|_.__/ \___|_|   
    //  / _ \ _   _  ___ _ __ _   _ ___                                    
    // | | | | | | |/ _ \ '__| | | / __|                                   
    // | |_| | |_| |  __/ |  | |_| \__ \                                   
    //  \__\_\\__,_|\___|_|   \__, |___/                                   
    //                        |___/                                        

    /**
     * @brief Creates a new phone number entry in the database.
     *
     * Inserts a new record into the "PhoneNumber" table with the provided phone number.
     *
     * @param phoneNumber The phone number to be created as a PhoneNumberDTO object.
     */
    QUERY(createPhoneNumber,
        "INSERT INTO PhoneNumber (number) "
        "VALUES (:phoneNumber.number);",
        PARAM(oatpp::Object<PhoneNumberDTO>, phoneNumber));

    /**
     * @brief Updates an existing phone number entry in the database.
     *
     * Updates the record in the "PhoneNumber" table with the provided phone number ID.
     *
     * @param phoneNumber The phone number information to update as a PhoneNumberDTO object.
     */
    QUERY(updatePhoneNumber,
        "UPDATE PhoneNumber "
        "SET "
        "number=:phoneNumber.number "
        "WHERE "
        "id=:phoneNumber.id;",
        PARAM(oatpp::Object<PhoneNumberDTO>, phoneNumber));

    /**
     * @brief Retrieves a phone number entry from the database by ID.
     *
     * Retrieves the record from the "PhoneNumber" table with the specified ID.
     *
     * @param id The ID of the phone number to retrieve.
     */
    QUERY(getPhoneNumberById,
        "SELECT * FROM PhoneNumber WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all phone number entries from the database.
     *
     * Retrieves records from the "PhoneNumber" table with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllPhoneNumbers,
        "SELECT * FROM PhoneNumber LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes a phone number entry from the database by ID.
     *
     * Deletes the record from the "PhoneNumber" table with the specified ID.
     *
     * @param id The ID of the phone number to delete.
     */
    QUERY(deletePhoneNumberById,
        "DELETE FROM PhoneNumber WHERE id=:id;",
        PARAM(oatpp::Int64, id));


    //  _____      __  __       _ _    ___                            
    // | ____|    |  \/  | __ _(_) |  / _ \ _   _  ___ _ __ _   _ ___ 
    // |  _| _____| |\/| |/ _` | | | | | | | | | |/ _ \ '__| | | / __|
    // | |__|_____| |  | | (_| | | | | |_| | |_| |  __/ |  | |_| \__ \
    // |_____|    |_|  |_|\__,_|_|_|  \__\_\\__,_|\___|_|   \__, |___/
    //                                                      |___/     

    /**
    * @brief Creates a new email entry in the database.
    *
    * Inserts a new record into the "Email" table with the provided email address.
    *
    * @param email The email address to be created as an EmailDTO object.
    */
    QUERY(createEmail,
        "INSERT INTO Email (email) "
        "VALUES (:email.email);",
        PARAM(oatpp::Object<EmailDTO>, email));

    /**
     * @brief Updates an existing email entry in the database.
     *
     * Updates the record in the "Email" table with the provided email ID.
     *
     * @param email The email address information to update as an EmailDTO object.
     */
    QUERY(updateEmail,
        "UPDATE Email "
        "SET "
        "email=:email.email "
        "WHERE "
        "id=:email.id;",
        PARAM(oatpp::Object<EmailDTO>, email));

    /**
     * @brief Retrieves an email entry from the database by ID.
     *
     * Retrieves the record from the "Email" table with the specified ID.
     *
     * @param id The ID of the email to retrieve.
     */
    QUERY(getEmailById,
        "SELECT * FROM Email WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all email entries from the database.
     *
     * Retrieves records from the "Email" table with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllEmails,
        "SELECT * FROM Email LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes an email entry from the database by ID.
     *
     * Deletes the record from the "Email" table with the specified ID.
     *
     * @param id The ID of the email to delete.
     */
    QUERY(deleteEmailById,
        "DELETE FROM Email WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves the ID of an email entry from the database by email address.
     *
     * Retrieves the ID from the "Email" table for the email address provided.
     *
     * @param email The email address to search for.
     */
    QUERY(getEmailIdByEmail,
        "SELECT id FROM Email WHERE email=:email;",
        PARAM(oatpp::String, email));

    //  __  __                _                   _     _       
    // |  \/  | ___ _ __ ___ | |__   ___ _ __ ___| |__ (_)_ __  
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__/ __| '_ \| | '_ \ 
    // | |  | |  __/ | | | | | |_) |  __/ |  \__ \ | | | | |_) |
    // |_|__|_|\___|_| |_| |_|_.__/ \___|_|  |___/_| |_|_| .__/ 
    //  / _ \ _   _  ___ _ __ _   _ ___                  |_|    
    // | | | | | | |/ _ \ '__| | | / __|                        
    // | |_| | |_| |  __/ |  | |_| \__ \                        
    //  \__\_\\__,_|\___|_|   \__, |___/                        
    //                        |___/                             

    /**
     * @brief Creates a new membership entry in the database.
     *
     * Inserts a new record into the "Membership" table with the provided membership fee and active status.
     *
     * @param membership The membership information to be created as a MembershipDTO object.
     */
    QUERY(createMembership,
        "INSERT INTO Membership (membershipFee, active) "
        "VALUES (:membership.membershipFee, 1);",
        PARAM(oatpp::Object<MembershipDTO>, membership));

    /**
     * @brief Updates an existing membership entry in the database.
     *
     * Updates the record in the "Membership" table with the provided membership ID.
     *
     * @param membership The membership information to update as a MembershipDTO object.
     */
    QUERY(updateMembership,
        "UPDATE Membership "
        "SET "
        "membershipFee=:membership.membershipFee, "
        "active=:membership.active "
        "WHERE "
        "id=:membership.id;",
        PARAM(oatpp::Object<MembershipDTO>, membership));

    /**
     * @brief Retrieves a membership entry from the database by ID.
     *
     * Retrieves the record from the "Membership" table with the specified ID.
     *
     * @param id The ID of the membership to retrieve.
     */
    QUERY(getMembershipById,
        "SELECT * FROM Membership WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all membership entries from the database.
     *
     * Retrieves records from the "Membership" table with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllMemberships,
        "SELECT * FROM Membership LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes a membership entry from the database by ID.
     *
     * Deletes the record from the "Membership" table with the specified ID.
     *
     * @param id The ID of the membership to delete.
     */
    QUERY(deleteMembershipById,
        "DELETE FROM Membership WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Deactivates a membership entry in the database by ID.
     *
     * Sets the 'active' flag to 0 for the membership record in the "Membership" table with the specified ID.
     *
     * @param id The ID of the membership to deactivate.
     */
    QUERY(deactivateMembership,
        "UPDATE Membership SET active=0 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Activates a membership entry in the database by ID.
     *
     * Sets the 'active' flag to 1 for the membership record in the "Membership" table with the specified ID.
     *
     * @param id The ID of the membership to activate.
     */
    QUERY(activateMembership,
        "UPDATE Membership SET active=1 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    //  ____                        _                        _   
    // |  _ \  ___ _ __   __ _ _ __| |_ _ __ ___   ___ _ __ | |_ 
    // | | | |/ _ \ '_ \ / _` | '__| __| '_ ` _ \ / _ \ '_ \| __|
    // | |_| |  __/ |_) | (_| | |  | |_| | | | | |  __/ | | | |_ 
    // |____/ \___| .__/ \__,_|_|   \__|_| |_| |_|\___|_| |_|\__|
    //  / _ \ _   |_|___ _ __ _   _ ___                          
    // | | | | | | |/ _ \ '__| | | / __|                         
    // | |_| | |_| |  __/ |  | |_| \__ \                         
    //  \__\_\\__,_|\___|_|   \__, |___/                         
    //                        |___/                              

    /**
    * @brief Creates a new department entry in the database.
    *
    * Inserts a new record into the "Department" table with the provided department name and active status.
    *
    * @param department The department information to be created as a DepartmentDTO object.
    */
    QUERY(createDepartment,
        "INSERT INTO Department (name, active) "
        "VALUES (:department.name, 1);",
        PARAM(oatpp::Object<DepartmentDTO>, department));

    /**
     * @brief Updates an existing department entry in the database.
     *
     * Updates the record in the "Department" table with the provided department ID.
     *
     * @param department The department information to update as a DepartmentDTO object.
     */
    QUERY(updateDepartment,
        "UPDATE Department "
        "SET "
        "name=:department.name, "
        "active=:department.active "
        "WHERE "
        "id=:department.id;",
        PARAM(oatpp::Object<DepartmentDTO>, department));

    /**
     * @brief Retrieves a department entry from the database by ID.
     *
     * Retrieves the record from the "Department" table with the specified ID.
     *
     * @param id The ID of the department to retrieve.
     */
    QUERY(getDepartmentById,
        "SELECT * FROM Department WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all active department entries from the database.
     *
     * Retrieves records from the "Department" table where the 'active' column is set to 1,
     * with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllDepartments,
        "SELECT * FROM Department WHERE active=1 LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Retrieves all inactive department entries from the database.
     *
     * Retrieves records from the "Department" table where the 'active' column is set to 0,
     * with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getInactiveDepartments,
        "SELECT * FROM Department WHERE active=0 LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes a department entry from the database by ID.
     *
     * Deletes the record from the "Department" table with the specified ID.
     *
     * @param id The ID of the department to delete.
     */
    QUERY(deleteDepartmentById,
        "DELETE FROM Department WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Deactivates a department entry in the database by ID.
     *
     * Sets the 'active' flag to 0 for the department record in the "Department" table with the specified ID.
     *
     * @param id The ID of the department to deactivate.
     */
    QUERY(deactivateDepartment,
        "UPDATE Department SET active=0 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Activates a department entry in the database by ID.
     *
     * Sets the 'active' flag to 1 for the department record in the "Department" table with the specified ID.
     *
     * @param id The ID of the department to activate.
     */
    QUERY(activateDepartment,
        "UPDATE Department SET active=1 WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    //  _____          _       _                ___                            
    // |_   _| __ __ _(_)_ __ (_)_ __   __ _   / _ \ _   _  ___ _ __ _   _ ___ 
    //   | || '__/ _` | | '_ \| | '_ \ / _` | | | | | | | |/ _ \ '__| | | / __|
    //   | || | | (_| | | | | | | | | | (_| | | |_| | |_| |  __/ |  | |_| \__ \
    //   |_||_|  \__,_|_|_| |_|_|_| |_|\__, |  \__\_\\__,_|\___|_|   \__, |___/
    //                                 |___/                         |___/     

    /**
    * @brief Creates a new training entry in the database.
    *
    * Inserts a new record into the "Training" table with the provided training information.
    *
    * @param training The training information to be created as a TrainingDTO object.
    */
    QUERY(createTraining,
        "INSERT INTO Training (date, startTime, room, notes) "
        "VALUES (:training.date, :training.startTime, :training.room, :training.notes);",
        PARAM(oatpp::Object<TrainingDTO>, training));

    /**
     * @brief Updates an existing training entry in the database.
     *
     * Updates the record in the "Training" table with the provided training ID.
     *
     * @param training The training information to update as a TrainingDTO object.
     */
    QUERY(updateTraining,
        "UPDATE Training "
        "SET "
        "date=:training.date, "
        "startTime=:training.startTime, "
        "room=:training.room, "
        "notes=:training.notes "
        "WHERE "
        "id=:training.id;",
        PARAM(oatpp::Object<TrainingDTO>, training));

    /**
     * @brief Retrieves a training entry from the database by ID.
     *
     * Retrieves the record from the "Training" table with the specified ID.
     *
     * @param id The ID of the training to retrieve.
     */
    QUERY(getTrainingById,
        "SELECT * FROM Training WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    /**
     * @brief Retrieves all training entries from the database.
     *
     * Retrieves records from the "Training" table with a specified limit and offset for pagination.
     *
     * @param offset The offset for pagination.
     * @param limit The maximum number of records to retrieve.
     */
    QUERY(getAllTrainings,
        "SELECT * FROM Training LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit));

    /**
     * @brief Deletes a training entry from the database by ID.
     *
     * Deletes the record from the "Training" table with the specified ID.
     *
     * @param id The ID of the training to delete.
     */
    QUERY(deleteTrainingById,
        "DELETE FROM Training WHERE id=:id;",
        PARAM(oatpp::Int64, id));

    //  ____      _       _   _                   _ 
    // |  _ \ ___| | __ _| |_(_) ___  _ __   __ _| |
    // | |_) / _ \ |/ _` | __| |/ _ \| '_ \ / _` | |
    // |  _ <  __/ | (_| | |_| | (_) | | | | (_| | |
    // |_|_\_\___|_|\__,_|\__|_|\___/|_| |_|\__,_|_|
    //  / _ \ _   _  ___ _ __ _   _ ___             
    // | | | | | | |/ _ \ '__| | | / __|            
    // | |_| | |_| |  __/ |  | |_| \__ \            
    //  \__\_\\__,_|\___|_|   \__, |___/            
    //                        |___/                 

    // Querys requesting stuff from multiple tables

    //  __  __                _                      _       _                   
    // |  \/  | ___ _ __ ___ | |__   ___ _ __       / \   __| |_ __ ___  ___ ___ 
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__|____ / _ \ / _` | '__/ _ \/ __/ __|
    // | |  | |  __/ | | | | | |_) |  __/ | |_____/ ___ \ (_| | | |  __/\__ \__ \
    // |_|__|_|\___|_| |_| |_|_.__/ \___|_|      /_/   \_\__,_|_|  \___||___/___/
    //  / _ \ _   _  ___ _ __ _   _ ___                                          
    // | | | | | | |/ _ \ '__| | | / __|                                         
    // | |_| | |_| |  __/ |  | |_| \__ \                                         
    //  \__\_\\__,_|\___|_|   \__, |___/                                         
    //                        |___/                                              


    /**
    * @brief Creates a new relationship between a member and an address in the database.
    *
    * Inserts a new record into the "MemberAddressRel" table with the provided member and address IDs.
    *
    * @param rel The member-address relationship information to be created as a MemberAdressRelDTO object.
    */
    QUERY(createMemberAddressRel,
        "INSERT INTO MemberAddressRel (memberID, addressID) "
        "VALUES (:rel.memberID, :rel.addressID);",
        PARAM(oatpp::Object<MemberAdressRelDTO>, rel));

    /**
     * @brief Retrieves the address IDs associated with a member ID from the database.
     *
     * Retrieves the address IDs from the "MemberAddressRel" table for the specified member ID.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getAddressIdsByMemberId,
        "SELECT addressID FROM MemberAddressRel WHERE memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves the member IDs associated with an address ID from the database.
     *
     * Retrieves the member IDs from the "MemberAddressRel" table for the specified address ID.
     *
     * @param addressID The ID of the address.
     */
    QUERY(getMemberIdsByAddressId,
        "SELECT memberID FROM MemberAddressRel WHERE addressID=:addressID;",
        PARAM(oatpp::Int64, addressID));

    /**
     * @brief Deletes a member-address relationship from the database.
     *
     * Deletes the record from the "MemberAddressRel" table with the specified member and address IDs.
     *
     * @param memberID The ID of the member.
     * @param addressID The ID of the address.
     */
    QUERY(deleteMemberAddressRel,
        "DELETE FROM MemberAddressRel WHERE memberID=:memberID AND addressID=:addressID;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::Int64, addressID));

    //  __  __                _                    _____                 _ _ 
    // |  \/  | ___ _ __ ___ | |__   ___ _ __     | ____|_ __ ___   __ _(_) |
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__|____|  _| | '_ ` _ \ / _` | | |
    // | |  | |  __/ | | | | | |_) |  __/ | |_____| |___| | | | | | (_| | | |
    // |_|__|_|\___|_| |_| |_|_.__/ \___|_|       |_____|_| |_| |_|\__,_|_|_|
    //  / _ \ _   _  ___ _ __ _   _ ___                                      
    // | | | | | | |/ _ \ '__| | | / __|                                     
    // | |_| | |_| |  __/ |  | |_| \__ \                                     
    //  \__\_\\__,_|\___|_|   \__, |___/                                     
    //                        |___/                                          

    /**
    * @brief Creates a new relationship between a member and an email in the database.
    *
    * Inserts a new record into the "MemberEmailRel" table with the provided member and email IDs.
    *
    * @param rel The member-email relationship information to be created as a MemberEmailRelDTO object.
    */
    QUERY(createMemberEmailRel,
        "INSERT INTO MemberEmailRel (memberID, emailID) "
        "VALUES (:rel.memberID, :rel.emailID);",
        PARAM(oatpp::Object<MemberEmailRelDTO>, rel));

    /**
     * @brief Retrieves the email IDs associated with a member ID from the database.
     *
     * Retrieves the email IDs from the "MemberEmailRel" table for the specified member ID.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getEmailIdsByMemberId,
        "SELECT emailID FROM MemberEmailRel WHERE memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves the member IDs associated with an email ID from the database.
     *
     * Retrieves the member IDs from the "MemberEmailRel" table for the specified email ID.
     *
     * @param emailID The ID of the email.
     */
    QUERY(getMemberIdsByEmailId,
        "SELECT memberID FROM MemberEmailRel WHERE emailID=:emailID;",
        PARAM(oatpp::Int64, emailID));

    /**
     * @brief Deletes a member-email relationship from the database.
     *
     * Deletes the record from the "MemberEmailRel" table with the specified member and email IDs.
     *
     * @param memberID The ID of the member.
     * @param emailID The ID of the email.
     */
    QUERY(deleteMemberEmailRel,
        "DELETE FROM MemberEmailRel WHERE memberID=:memberID AND emailID=:emailID;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::Int64, emailID));


    //  __  __                _                    ____  _                            
    // |  \/  | ___ _ __ ___ | |__   ___ _ __     |  _ \| |__   ___  _ __   ___ _ __  
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__|____| |_) | '_ \ / _ \| '_ \ / _ \ '_ \ 
    // | |  | |  __/ | | | | | |_) |  __/ | |_____|  __/| | | | (_) | | | |  __/ | | |
    // |_|  |_|\___|_| |_| |_|_.__/ \___|_|___    |_|   |_| |_|\___/|_| |_|\___|_| |_|
    //  _   _ _ __ ___ | |__   ___ _ __   / _ \ _   _  ___ _ __ _   _ ___             
    // | | | | '_ ` _ \| '_ \ / _ \ '__| | | | | | | |/ _ \ '__| | | / __|            
    // | |_| | | | | | | |_) |  __/ |    | |_| | |_| |  __/ |  | |_| \__ \            
    //  \__,_|_| |_| |_|_.__/ \___|_|     \__\_\\__,_|\___|_|   \__, |___/            
    //                                                          |___/                 

    /**
    * @brief Creates a new relationship between a member and a phone number in the database.
    *
    * Inserts a new record into the "MemberPhoneNumberRel" table with the provided member and phone number IDs.
    *
    * @param rel The member-phone number relationship information to be created as a MemberPhonenumberRelDTO object.
    */
    QUERY(createMemberPhoneNumberRel,
        "INSERT INTO MemberPhoneNumberRel (memberID, phoneNumberID) "
        "VALUES (:rel.memberID, :rel.phoneNumberID);",
        PARAM(oatpp::Object<MemberPhonenumberRelDTO>, rel));

    /**
     * @brief Retrieves the phone number IDs associated with a member ID from the database.
     *
     * Retrieves the phone number IDs from the "MemberPhoneNumberRel" table for the specified member ID.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getPhoneNumberIdsByMemberId,
        "SELECT phoneNumberID FROM MemberPhoneNumberRel WHERE memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves the member IDs associated with a phone number ID from the database.
     *
     * Retrieves the member IDs from the "MemberPhoneNumberRel" table for the specified phone number ID.
     *
     * @param phoneNumberID The ID of the phone number.
     */
    QUERY(getMemberIdsByPhoneNumberId,
        "SELECT memberID FROM MemberPhoneNumberRel WHERE phoneNumberID=:phoneNumberID;",
        PARAM(oatpp::Int64, phoneNumberID));

    /**
     * @brief Deletes a member-phone number relationship from the database.
     *
     * Deletes the record from the "MemberPhoneNumberRel" table with the specified member and phone number IDs.
     *
     * @param memberID The ID of the member.
     * @param phoneNumberID The ID of the phone number.
     */
    QUERY(deleteMemberPhoneNumberRel,
        "DELETE FROM MemberPhoneNumberRel WHERE memberID=:memberID AND phoneNumberID=:phoneNumberID;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::Int64, phoneNumberID));



    //  __  __                _                    __  __                _                   _     _       
    // |  \/  | ___ _ __ ___ | |__   ___ _ __     |  \/  | ___ _ __ ___ | |__   ___ _ __ ___| |__ (_)_ __  
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__|____| |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__/ __| '_ \| | '_ \ 
    // | |  | |  __/ | | | | | |_) |  __/ | |_____| |  | |  __/ | | | | | |_) |  __/ |  \__ \ | | | | |_) |
    // |_|__|_|\___|_| |_| |_|_.__/ \___|_|       |_|  |_|\___|_| |_| |_|_.__/ \___|_|  |___/_| |_|_| .__/ 
    //  / _ \ _   _  ___ _ __ _   _ ___                                                             |_|    
    // | | | | | | |/ _ \ '__| | | / __|                                                                   
    // | |_| | |_| |  __/ |  | |_| \__ \                                                                   
    //  \__\_\\__,_|\___|_|   \__, |___/                                                                   
    //                        |___/                                                                        

    /**
    * @brief Creates a new relationship between a member and a membership in the database.
    *
    * Inserts a new record into the "MemberMembershipRel" table with the provided member and membership IDs.
    *
    * @param rel The member-membership relationship information to be created as a MemberMembershipRelDTO object.
    */
    QUERY(createMemberMembershipRel,
        "INSERT INTO MemberMembershipRel (memberID, membershipID) "
        "VALUES (:rel.memberID, :rel.membershipID);",
        PARAM(oatpp::Object<MemberMembershipRelDTO>, rel));

    /**
     * @brief Retrieves the membership IDs associated with a member ID from the database.
     *
     * Retrieves the membership IDs from the "MemberMembershipRel" table for the specified member ID.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getMembershipIdsByMemberId,
        "SELECT membershipID FROM MemberMembershipRel WHERE memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves the member IDs associated with a membership ID from the database.
     *
     * Retrieves the member IDs from the "MemberMembershipRel" table for the specified membership ID.
     *
     * @param membershipID The ID of the membership.
     */
    QUERY(getMemberIdsByMembershipId,
        "SELECT memberID FROM MemberMembershipRel WHERE membershipID=:membershipID;",
        PARAM(oatpp::Int64, membershipID));

    /**
     * @brief Deletes a member-membership relationship from the database.
     *
     * Deletes the record from the "MemberMembershipRel" table with the specified member and membership IDs.
     *
     * @param memberID The ID of the member.
     * @param membershipID The ID of the membership.
     */
    QUERY(deleteMemberMembershipRel,
        "DELETE FROM MemberMembershipRel WHERE memberID=:memberID AND membershipID=:membershipID;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::Int64, membershipID));



    //  __  __                _                  _____          _       _             
    // |  \/  | ___ _ __ ___ | |__   ___ _ __   |_   _| __ __ _(_)_ __ (_)_ __   __ _ 
    // | |\/| |/ _ \ '_ ` _ \| '_ \ / _ \ '__|____| || '__/ _` | | '_ \| | '_ \ / _` |
    // | |  | |  __/ | | | | | |_) |  __/ | |_____| || | | (_| | | | | | | | | | (_| |
    // |_|__|_|\___|_| |_| |_|_.__/ \___|_|       |_||_|  \__,_|_|_| |_|_|_| |_|\__, |
    //  / _ \ _   _  ___ _ __ _   _ ___                                         |___/ 
    // | | | | | | |/ _ \ '__| | | / __|                                              
    // | |_| | |_| |  __/ |  | |_| \__ \                                              
    //  \__\_\\__,_|\___|_|   \__, |___/                                              
    //                        |___/                                                   

    /**
    * @brief Creates a new relationship between a member and a training in the database.
    *
    * Inserts a new record into the "MemberTrainingRel" table with the provided member and training IDs.
    *
    * @param rel The member-training relationship information to be created as a MemberTrainingRelDTO object.
    */
    QUERY(createMemberTrainingRel,
        "INSERT INTO MemberTrainingRel (memberID, trainingID) "
        "VALUES (:rel.memberID, :rel.trainingID);",
        PARAM(oatpp::Object<MemberTrainingRelDTO>, rel));

    /**
     * @brief Retrieves the training IDs associated with a member ID from the database.
     *
     * Retrieves the training IDs from the "MemberTrainingRel" table for the specified member ID.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getTrainingIdsByMemberId,
        "SELECT trainingID FROM MemberTrainingRel WHERE memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves the member IDs associated with a training ID from the database.
     *
     * Retrieves the member IDs from the "MemberTrainingRel" table for the specified training ID.
     *
     * @param trainingID The ID of the training.
     */
    QUERY(getMemberIdsByTrainingId,
        "SELECT memberID FROM MemberTrainingRel WHERE trainingID=:trainingID;",
        PARAM(oatpp::Int64, trainingID));

    /**
     * @brief Deletes a member-training relationship from the database.
     *
     * Deletes the record from the "MemberTrainingRel" table with the specified member and training IDs.
     *
     * @param memberID The ID of the member.
     * @param trainingID The ID of the training.
     */
    QUERY(deleteMemberTrainingRel,
        "DELETE FROM MemberTrainingRel WHERE memberID=:memberID AND trainingID=:trainingID;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::Int64, trainingID));



    //  ____                        _                        _      _____          _       _             
    // |  _ \  ___ _ __   __ _ _ __| |_ _ __ ___   ___ _ __ | |_   |_   _| __ __ _(_)_ __ (_)_ __   __ _ 
    // | | | |/ _ \ '_ \ / _` | '__| __| '_ ` _ \ / _ \ '_ \| __|____| || '__/ _` | | '_ \| | '_ \ / _` |
    // | |_| |  __/ |_) | (_| | |  | |_| | | | | |  __/ | | | ||_____| || | | (_| | | | | | | | | | (_| |
    // |____/ \___| .__/ \__,_|_|   \__|_| |_| |_|\___|_| |_|\__|    |_||_|  \__,_|_|_| |_|_|_| |_|\__, |
    //  / _ \ _   |_|___ _ __ _   _ ___                                                            |___/ 
    // | | | | | | |/ _ \ '__| | | / __|                                                                 
    // | |_| | |_| |  __/ |  | |_| \__ \                                                                 
    //  \__\_\\__,_|\___|_|   \__, |___/                                                                 
    //                        |___/                                                                      

    /**
    * @brief Creates a new relationship between a department and a training in the database.
    *
    * Inserts a new record into the "DepartmentTrainingRel" table with the provided department and training IDs.
    *
    * @param rel The department-training relationship information to be created as a DepartmentTrainingRelDTO object.
    */
    QUERY(createDepartmentTrainingRel,
        "INSERT INTO DepartmentTrainingRel (departmentID, trainingID) "
        "VALUES (:rel.departmentID, :rel.trainingID);",
        PARAM(oatpp::Object<DepartmentTrainingRelDTO>, rel));

    /**
     * @brief Retrieves the training IDs associated with a department ID from the database.
     *
     * Retrieves the training IDs from the "DepartmentTrainingRel" table for the specified department ID.
     *
     * @param departmentID The ID of the department.
     */
    QUERY(getTrainingIdsByDepartmentId,
        "SELECT trainingID FROM DepartmentTrainingRel WHERE departmentID=:departmentID;",
        PARAM(oatpp::Int64, departmentID));

    /**
     * @brief Retrieves the department IDs associated with a training ID from the database.
     *
     * Retrieves the department IDs from the "DepartmentTrainingRel" table for the specified training ID.
     *
     * @param trainingID The ID of the training.
     */
    QUERY(getDepartmentIdsByTrainingId,
        "SELECT departmentID FROM DepartmentTrainingRel WHERE trainingID=:trainingID;",
        PARAM(oatpp::Int64, trainingID));

    /**
     * @brief Deletes a department-training relationship from the database.
     *
     * Deletes the record from the "DepartmentTrainingRel" table with the specified department and training IDs.
     *
     * @param departmentID The ID of the department.
     * @param trainingID The ID of the training.
     */
    QUERY(deleteDepartmentTrainingRel,
        "DELETE FROM DepartmentTrainingRel WHERE departmentID=:departmentID AND trainingID=:trainingID;",
        PARAM(oatpp::Int64, departmentID),
        PARAM(oatpp::Int64, trainingID));


    //      _       _                   
    //     | | ___ (_)_ __              
    //  _  | |/ _ \| | '_ \             
    // | |_| | (_) | | | | |            
    //  \___/ \___/|_|_| |_|            
    //  / _ \ _   _  ___ _ __ _   _ ___ 
    // | | | | | | |/ _ \ '__| | | / __|
    // | |_| | |_| |  __/ |  | |_| \__ \
    //  \__\_\\__,_|\___|_|   \__, |___/
    //                        |___/     

    /**
    * @brief Retrieves trainings associated with a member ID from the database.
    *
    * Retrieves the details of trainings attended by the specified member from the "Training" table.
    *
    * @param memberID The ID of the member.
    */
    QUERY(getTrainingsByMemberId,
        "SELECT Training.* FROM Training "
        "JOIN MemberTrainingRel ON Training.id = MemberTrainingRel.trainingID "
        "WHERE MemberTrainingRel.memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves memberships associated with a member ID from the database.
     *
     * Retrieves the details of memberships held by the specified member from the "Membership" table.
     *
     * @param memberID The ID of the member.
     */
    QUERY(getMembershipsByMemberId,
        "SELECT Membership.* FROM Membership "
        "JOIN MemberMembershipRel ON Membership.id = MemberMembershipRel.membershipID "
        "WHERE MemberMembershipRel.memberID=:memberID;",
        PARAM(oatpp::Int64, memberID));

    /**
     * @brief Retrieves members associated with a department ID from the database.
     *
     * Retrieves the details of members belonging to the specified department from the "Member" table.
     *
     * @param departmentID The ID of the department.
     */
    QUERY(getMembersByDepartmentId,
        "SELECT Member.* FROM Member "
        "JOIN MemberDepartmentRel ON Member.id = MemberDepartmentRel.memberID "
        "WHERE MemberDepartmentRel.departmentID=:departmentID;",
        PARAM(oatpp::Int64, departmentID));

    /**
     * @brief Retrieves trainings associated with a department ID from the database.
     *
     * Retrieves the details of trainings associated with the specified department from the "Training" table.
     *
     * @param departmentID The ID of the department.
     */
    QUERY(getTrainingsByDepartmentId,
        "SELECT Training.* FROM Training "
        "JOIN DepartmentTrainingRel ON Training.id = DepartmentTrainingRel.trainingID "
        "WHERE DepartmentTrainingRel.departmentID=:departmentID;",
        PARAM(oatpp::Int64, departmentID));

    /**
     * @brief Retrieves trainings associated with a member ID within a specified time range from the database.
     *
     * Retrieves the details of trainings attended by the specified member within the specified time range from the "Training" table.
     *
     * @param memberID The ID of the member.
     * @param startDate The start date of the time range.
     * @param endDate The end date of the time range.
     */
    QUERY(getTrainingsByMemberIdAndTimeRange,
        "SELECT Training.* FROM Training "
        "JOIN MemberTrainingRel ON Training.id = MemberTrainingRel.trainingID "
        "WHERE MemberTrainingRel.memberID=:memberID AND Training.date BETWEEN :startDate AND :endDate;",
        PARAM(oatpp::Int64, memberID),
        PARAM(oatpp::String, startDate),
        PARAM(oatpp::String, endDate));

    /**
     * @brief Retrieves members associated with a training ID from the database.
     *
     * Retrieves the details of members attending the specified training from the "Member" table.
     *
     * @param trainingID The ID of the training.
     */
    QUERY(getMembersByTrainingId,
        "SELECT Member.* FROM Member "
        "JOIN MemberTrainingRel ON Member.id = MemberTrainingRel.memberID "
        "WHERE MemberTrainingRel.trainingID=:trainingID",
        PARAM(oatpp::Int64, trainingID))

    /**
     * @brief Retrieves members with yearly attendance count from the database.
     *
     * Retrieves the details of members who attended at least 18 trainings per year from the "Member" table.
     */
    QUERY(getMembersWithYearlyAttendance,
        "SELECT Member.*, COUNT(Training.id) AS attendance_count "
        "FROM Member "
        "JOIN MemberTrainingRel ON Member.id = MemberTrainingRel.memberID "
        "JOIN Training ON MemberTrainingRel.trainingID = Training.id "
        "GROUP BY Member.id "
        "HAVING attendance_count >= 18;");

    /**
     * @brief Retrieves members with monthly attendance count from the database.
     *
     * Retrieves the details of members who attended at least 12 distinct months of trainings from the "Member" table.
     */
    QUERY(getMembersWithMonthlyAttendance,
        "SELECT Member.*, "
        "COUNT(DISTINCT STRFTIME('%Y-%m', Training.date)) AS unique_month_count "
        "FROM Member "
        "JOIN MemberTrainingRel ON Member.id = MemberTrainingRel.memberID "
        "JOIN Training ON MemberTrainingRel.trainingID = Training.id "
        "GROUP BY Member.id "
        "HAVING unique_month_count >= 12;");


};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section

#endif //DATABASE_CLIENT