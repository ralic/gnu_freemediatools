/*******************************************************************************
**
** The source file for the Jahshaka glworld dataio code
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/



#include "glworld.h"
#include "glworldlists.h"
#include "jahdataio.h"
#include "InputLCD.h"
#include "supergrangecontrol.h"
#include "eyeCheckListItem.h"
#include "jahloadxml.h"
#include "loadingdialog.h"
#include <openmedialib.h>
#include <qmessagebox.h>
#include <jahpluginlib.h>
#include <qlistview.h>

template < typename T > 
QCheckListItem* 
factory__( T* parent, QListViewItem* after, const QString& s )
{
    if ( after )
	return new EyeCheckListItem( parent, after, s, QCheckListItem::CheckBox );

    return new EyeCheckListItem( parent, s, QCheckListItem::CheckBox );
}


void 
GLWorld::loadSceneFile(std::string file_name )
{
    char* scene_file_buffer;

    if ( JahDataIO::readFileIntoBuffer( scene_file_buffer, (char*)file_name.c_str() ) )
    {
        std::string scene_file_string = scene_file_buffer;
        LoadAll(scene_file_string, true, false, file_name);
    }

    updateTimeSlider(m_nMinFrame);
}

bool GLWorld::LoadAll( std::string const& scene_file_string, bool clear_first, bool apply_effects_only, std::string const& file_name_string ) 
{ 
    //grab a copy of this for the dialog
    QString loadFileName( scene_file_string.data() );
    
    int last_slash = (int)file_name_string.find_last_of("/");
    std::string load_directory = file_name_string.substr(0, last_slash + 1);


    SceneFileXml scene_file_xml(scene_file_string);

    int string_position = 0;
    SceneFileXml::KEYWORD keyword;
    bool use_relative_paths = false;

    string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::JAHSHAKA);
    scene_file_xml.parseJahshakaStructure( scene_file_xml.getJahshakaStructure() );

    //some error checking here
    if ( scene_file_xml.getJahshakaStructure().jah_version.find("1.9") == std::string::npos &&
        scene_file_xml.getJahshakaStructure().jah_version.find("2.0") == std::string::npos )
    {
        if ( !Globals::getSuppressPopups() )
        {
            QMessageBox::information(0, "Error loading Scene file", "Incorrect jah_version");
        }

        return false;
    }


    string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::SCENE_FILE);
    scene_file_xml.parseSceneFileStructure( scene_file_xml.getSceneFileStructure() );

    string_position = scene_file_xml.goToNextUnknownKeywordPosition(keyword);
    
    if (keyword == SceneFileXml::EXPORT)
    {
        use_relative_paths = true;
        string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::MODULE);
    }

    scene_file_xml.parseModuleStructure( scene_file_xml.getModuleStructure() );

    string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::FRAMES);
    scene_file_xml.parseFramesStructure( scene_file_xml.getFramesStructure() );

    std::string resolution_string = scene_file_xml.getFramesStructure().resolution;

    projectData project_data;
    int resolution_index = project_data.getFieldNumber(resolution_string);
    m_pJahResCombo->setCurrentItem(resolution_index);
    setResolution(resolution_index);

    if (clear_first)
    {
        m_nAAnimFrame = scene_file_xml.getFramesStructure().number;
        updatesliderEndframe(m_nAAnimFrame);
    }

#ifndef JAHPLAYER
    if (m_pEndFrameControl)
    {
        m_pEndFrameControl->setValue(m_nAAnimFrame);
    }
#endif

    // This is broken - copied from old code
    m_nJahResolutionValue = scene_file_xml.getFramesStructure().code;


    //we can only load a scene that was generated by a module into the same module
    std::string module_name = std::string( m_qsModuleName.data() );

    bool apply_effects_to_animation_layer = false;
    bool append_effects_scene_to_animation_module = false;

    if (scene_file_xml.getModuleStructure().name == "Effects" && 
        module_name == "Animation" && !clear_first)
    {
        if ( getActiveJahLayer() && canAcceptEffects(getActiveJahLayer()->objtype) && apply_effects_only)
        {
            apply_effects_to_animation_layer = true;
        }
        else
        {
            append_effects_scene_to_animation_module = true;
        }
    }

    if (   scene_file_xml.getModuleStructure().name == module_name 
        || apply_effects_to_animation_layer
        || append_effects_scene_to_animation_module
        ) 
    {

        ///////////////////////////////////////////////////////    
        //create the loading dialog
        loadingDialog * loadingscreen   = new loadingDialog();  //loadingDialog()
        loadingscreen->setActiveWindow();

        //set first dialog line
        loadingscreen->setDialog1Text("Loading "+ m_qsModuleName +" Scene File");

        //doesnt work for some reason loadFileName isnt filename?
        //loadingscreen->setDialog2Text(loadFileName);  

        loadingscreen->setProgressText("Loading");

        //used for the progress bar
        //this should be the total number of layers+number of effects we are loading
        int numberofloadingsteps = 5;

        //the first step
        int loadingstep = 0;

        ///////////////////////////////////////////////////////

        //useBuffering = false;

        if (clear_first)
        {
            //first clear the module and reset variables
            loadClearAll();
        }

        //this is also the same for color and keyer and tracker i believe
        //as well as cg/text mod and paint
        std::string module_name = scene_file_xml.getModuleStructure().name;

        if ( (module_name == "Effects" || 
              module_name == "Color" || 
              module_name == "Keyer" || 
              module_name == "Tracker") &&
            !(apply_effects_to_animation_layer || append_effects_scene_to_animation_module) )
        {
            JahLayer* jah_layer = configureLayer ("World", false, true, false, true );
            buildLayer(jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );
            m_pCamera = jah_layer;
        }

        LayerCategory::TYPE layer_category = LayerCategory::NOT_A_TYPE;
        ObjectCategory::TYPE object_category = ObjectCategory::NOT_A_TYPE;
        CompositeType::TYPE composite_type = CompositeType::STANDARD_COMPOSITE_TYPE;
        LayerListEntry* new_layer_list_entry;
        JahLayer* new_jah_layer;
        JahLayer* parent_layer;
        assetData asset_data;
        assetData key_asset_data;
        std::string layer_name;

        // Find the first layer
        string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::LAYER);

        while ( string_position != int(std::string::npos) )
        {
            //////////////////////////////////////////////////////////
            //increment loading dialog progress bar
            loadingstep++;
            loadingscreen->setProgressValue((100/numberofloadingsteps)*loadingstep);
            //////////////////////////////////////////////////////////

            // Found a layer
            scene_file_xml.parseLayerStructure( scene_file_xml.getLayerStructure() );
            QString layer_name_qstring = scene_file_xml.getLayerStructure().name.data();
            layer_name = scene_file_xml.getLayerStructure().name.data();

            if (layer_name == "World" && !clear_first)
            {
                // Don't create a world layer when appending
                string_position = scene_file_xml.goToNextKnownKeywordPosition(SceneFileXml::LAYER);
                continue;
            }

            if (!apply_effects_to_animation_layer)
            {
                new_layer_list_entry = new LayerListEntry(layer_name_qstring);
                getLayerList()->append(new_layer_list_entry);
                new_jah_layer = new_layer_list_entry->getJahLayer();
                new_jah_layer->layername = layer_name_qstring;
                new_jah_layer->m_layer_name_string = scene_file_xml.getLayerStructure().name.data();
                new_jah_layer->layerStatus = scene_file_xml.getLayerStructure().activation;
                QCheckListItem* new_q_check_list_item 
		  = addCheckListItem(
				     m_pLayerListView, 
				     new_jah_layer->layerStatus, 
				     false, 
				     &factory__<QListView>);
                new_jah_layer->setCheckListItem(new_q_check_list_item);

                // This layer becomes the current parent for effects
                parent_layer = new_jah_layer;
            }
            else
            {
                parent_layer = getActiveJahLayer();
                new_jah_layer = parent_layer;
            }

            if (layer_name == "World")
            {
                m_pCamera = new_jah_layer;
            }

            new_jah_layer->setCameraLayer(m_pCamera);

            scene_file_xml.getAssetStructure().layer_status = false;
            scene_file_xml.getKeyAssetStructure().layer_status = false;

            do
            {
                string_position = scene_file_xml.goToNextUnknownKeywordPosition(keyword);

                switch (keyword)
                {
                    case SceneFileXml::LAYER :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        break;
                    }
                    case SceneFileXml::ASSET :
                    {
                        if (apply_effects_to_animation_layer)
                        {
                            break;
                        }

                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseAssetStructure( scene_file_xml.getAssetStructure() );
                        asset_data.theCategory = scene_file_xml.getAssetStructure().category;
                        asset_data.theType = scene_file_xml.getAssetStructure().type;
                        asset_data.clipinframe = scene_file_xml.getAssetStructure().clipinframe;
                        asset_data.clipname = QString( scene_file_xml.getAssetStructure().clipname.data() );
                        asset_data.clipnumber = scene_file_xml.getAssetStructure().clipnumber;
                        asset_data.clipoutframe = scene_file_xml.getAssetStructure().clipoutframe;
                        asset_data.endframe = scene_file_xml.getAssetStructure().endframe;
                        asset_data.extension = QString( scene_file_xml.getAssetStructure().extension.data() );
                        asset_data.filename = QString( scene_file_xml.getAssetStructure().filename.data() );

                        if (use_relative_paths)
                        {
                            std::string asset_dir = load_directory;
                            
                            if (asset_data.theCategory == VideoCategory::CLIP)
                            {
                                asset_dir += scene_file_xml.getAssetStructure().filename;
                                asset_dir += "/";
                            }

                            asset_data.location = QString( asset_dir.data() );
                        }
                        else
                        {
                            asset_data.location = QString( scene_file_xml.getAssetStructure().location.data() );
                        }

                        asset_data.startframe = scene_file_xml.getAssetStructure().startframe;
                        asset_data.videotex = scene_file_xml.getAssetStructure().videotex;
                        asset_data.videoext = QString( scene_file_xml.getAssetStructure().videoext.data() );
                        new_jah_layer->m_asset_name_string = scene_file_xml.getAssetStructure().clipname.data();
                        new_jah_layer->setSlipFrameOffset( scene_file_xml.getAssetStructure().slip );
                        new_jah_layer->m_inframe = scene_file_xml.getAssetStructure().inframe_number;
                        new_jah_layer->m_outframe = scene_file_xml.getAssetStructure().outframe_number;
                        new_jah_layer->loop = scene_file_xml.getAssetStructure().loop;
                        new_jah_layer->ping = scene_file_xml.getAssetStructure().ping;
                        new_jah_layer->setInframeValid(true);
                        break;
                    }
                    case SceneFileXml::KEY_ASSET :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseKeyAssetStructure( scene_file_xml.getKeyAssetStructure() );
                        key_asset_data.theCategory = scene_file_xml.getKeyAssetStructure().category;
                        key_asset_data.theType = scene_file_xml.getKeyAssetStructure().type;
                        key_asset_data.clipinframe = scene_file_xml.getKeyAssetStructure().clipinframe;
                        key_asset_data.clipname = QString( scene_file_xml.getKeyAssetStructure().clipname.data() );
                        key_asset_data.clipnumber = scene_file_xml.getKeyAssetStructure().clipnumber;
                        key_asset_data.clipoutframe = scene_file_xml.getKeyAssetStructure().clipoutframe;
                        key_asset_data.endframe = scene_file_xml.getKeyAssetStructure().endframe;
                        key_asset_data.extension = QString( scene_file_xml.getKeyAssetStructure().extension.data() );
                        key_asset_data.filename = QString( scene_file_xml.getKeyAssetStructure().filename.data() );

                        if (use_relative_paths)
                        {
                            std::string key_asset_dir = load_directory;
                            
                            if (key_asset_data.theCategory == VideoCategory::CLIP)
                            {
                                key_asset_dir += scene_file_xml.getKeyAssetStructure().filename;
                                key_asset_dir += "/";
                            }

                            key_asset_data.location = QString( key_asset_dir.data() );
                        }
                        else
                        {
                            key_asset_data.location = QString( scene_file_xml.getKeyAssetStructure().location.data() );
                        }

                        key_asset_data.startframe = scene_file_xml.getKeyAssetStructure().startframe;
                        key_asset_data.videotex = scene_file_xml.getKeyAssetStructure().videotex;
                        key_asset_data.videoext = QString( scene_file_xml.getKeyAssetStructure().videoext.data() );
                        scene_file_xml.getKeyAssetStructure().layer_status = true;
                        std::string blendmode = scene_file_xml.getKeyAssetStructure().blendmode;
                        new_jah_layer->blendmode2 = SceneFileXml::getBlendmodeValue(blendmode);
                        new_jah_layer->setKeySlipFrameOffset( scene_file_xml.getKeyAssetStructure().slip );
                        new_jah_layer->m_keyinframe = scene_file_xml.getKeyAssetStructure().inframe_number;
                        new_jah_layer->m_keyoutframe = scene_file_xml.getKeyAssetStructure().outframe_number;
						break;
                    }
                    case SceneFileXml::PROPERTIES :
                    {
                        if (apply_effects_to_animation_layer)
                        {
                            break;
                        }

                        SceneFileXml::PROPERTIES_STRUCTURE& properties_structure = scene_file_xml.getPropertiesStructure();
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parsePropertiesStructure( properties_structure );
                        new_jah_layer->blend = properties_structure.blend;           
                        new_jah_layer->depth = properties_structure.depth;
                        new_jah_layer->cull = properties_structure.cull;
                        new_jah_layer->setLightingEnabled( properties_structure.lighting );
                        new_jah_layer->setTranslateFirst( properties_structure.translate_first );
                        new_jah_layer->foreground = properties_structure.foreground;
                        new_jah_layer->smooth = properties_structure.smooth;
                        new_jah_layer->reflect = properties_structure.reflect;
                        break;
                    }
                    case SceneFileXml::KEYFRAMES :
                    {
                        if (apply_effects_to_animation_layer)
                        {
                            break;
                        }

                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.getLayerStructure().keyframes.keyframe_vector.clear();
                        scene_file_xml.parseKeyframesStructure( scene_file_xml.getLayerStructure().keyframes );
                        scene_file_xml.setKeyframes(new_jah_layer, scene_file_xml.getLayerStructure().keyframes);
                        break;
                    }
                    case SceneFileXml::COLOR :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseColorStructure( scene_file_xml.getColorStructure() );
                        new_jah_layer->Red = scene_file_xml.getColorStructure().value.x;
                        new_jah_layer->Green = scene_file_xml.getColorStructure().value.y;
                        new_jah_layer->Blue = scene_file_xml.getColorStructure().value.z;
                        break;
                    }
                    case SceneFileXml::TEXT :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseTextStructure( scene_file_xml.getTextStructure() );
                        new_jah_layer->addTextObj();

                        if (use_relative_paths)
                        {
                            std::string full_font_name = scene_file_xml.getTextStructure().font;
                            int last_slash = (int)full_font_name.find_last_of("/");
                            int font_name_length = (int)full_font_name.length() - last_slash - 1;
                            std::string font_name = full_font_name.substr(last_slash + 1, font_name_length);
                            std::string font_path = load_directory;
                            font_path += font_name;
                            new_jah_layer->text->font = font_path.data();
                        }
                        else
                        {
                            new_jah_layer->text->font = scene_file_xml.getTextStructure().font.data();
                        }

                        new_jah_layer->text->text = scene_file_xml.getTextStructure().text.data();
                        new_jah_layer->text->fontstyle = scene_file_xml.getTextStructure().style;
                        new_jah_layer->layernodes->m_node->extrude = scene_file_xml.getTextStructure().extrude_depth;
                        setObjectTab( new_jah_layer );
                        break;
                    }
                    case SceneFileXml::KEY :
                    {
                        if (apply_effects_to_animation_layer)
                        {
                            break;
                        }

                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseKeyStructure( scene_file_xml.getKeyStructure() );
                        new_jah_layer->invertKey = scene_file_xml.getKeyStructure().invert;
                        new_jah_layer->drawtheKey = scene_file_xml.getKeyStructure().draw;
                        new_jah_layer->keyAssetStatus = scene_file_xml.getKeyStructure().status;
                        new_jah_layer->m_keyinframe = scene_file_xml.getKeyStructure().keyinframe;
                        new_jah_layer->m_keyoutframe = scene_file_xml.getKeyStructure().keyoutframe;
                        new_jah_layer->setUseOpenGlKeyer( scene_file_xml.getKeyStructure().use_open_gl_keyer );
                        break;
                    }
                    case SceneFileXml::OBJECT :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseObjectStructure( scene_file_xml.getObjectStructure() );

                        if (use_relative_paths)
                        {
                            std::string full_model_name = scene_file_xml.getObjectStructure().filename;
                            int last_slash = (int)full_model_name.find_last_of("/");
                            int model_name_length = (int)full_model_name.length() - last_slash - 1;
                            std::string model_name = full_model_name.substr(last_slash + 1, model_name_length);
                            std::string model_path = load_directory;
                            model_path += model_name;
                            new_jah_layer->objfilename = model_path.data();
                        }
                        else
                        {
                            new_jah_layer->objfilename = scene_file_xml.getObjectStructure().filename.data();
                        }

                        new_jah_layer->objextension = scene_file_xml.getObjectStructure().extension.data();
                        new_jah_layer->loadObject = true;
                        break;
                    }
                    case SceneFileXml::PARTICLE :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseParticleStructure( scene_file_xml.getParticleStructure() );
                        std::string type_name = scene_file_xml.getParticleStructure().type;
                        new_jah_layer->ParticleStyle = ParticleCategory::getType(type_name);
                        std::string draw_string = scene_file_xml.getParticleStructure().draw;
                        new_jah_layer->ParticleDraw = ParticleDrawstyle::getType(draw_string);
                        new_jah_layer->ParticleColors = scene_file_xml.getParticleStructure().color;
                        new_jah_layer->ParticleFirsttime = true;
                        new_jah_layer->blend = true;
                        new_jah_layer->depth = true;
                        new_jah_layer->lighting = false;
                        new_jah_layer->loadObject = true;
                        break;
                    }
                    case SceneFileXml::TYPE :
                    {
                        if (apply_effects_to_animation_layer)
                        {
                            break;
                        }

                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseTypeStructure( scene_file_xml.getTypeStructure() );
                        layer_category = LayerCategory::getType(scene_file_xml.getTypeStructure().layertype);
                        new_jah_layer->layertype = layer_category;
                        object_category = ObjectCategory::getType(scene_file_xml.getTypeStructure().objtype);
                        new_jah_layer->objtype = object_category;
                        composite_type = CompositeType::getType(scene_file_xml.getTypeStructure().composite_type);
                        new_jah_layer->setCompositeType(composite_type);

                        if (module_name == "Animation")
                        {
                            updateCompositeButton(composite_type);
                        }

                        break;
                    }
                    case SceneFileXml::BACKDROP :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.parseBackdropStructure( scene_file_xml.getBackdropStructure() );
                        m_bGridVal = (bool)scene_file_xml.getBackdropStructure().grid_setting;
                        m_bPolySmooth = scene_file_xml.getBackdropStructure().aliasing;
                        m_pTextSafe->layerStatus = scene_file_xml.getBackdropStructure().text_safe;
                        break;
                    }
                    case SceneFileXml::LIGHT :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.getLightStructure().keyframes.keyframe_vector.clear();
                        scene_file_xml.parseLightStructure( scene_file_xml.getLightStructure() );

                        if (module_name != "Animation")
                        {
                            break;
                        }

                        JahLayer* light_layer = createALayer(LAYER_LIGHT, new_jah_layer);
                        scene_file_xml.setKeyframes(light_layer, scene_file_xml.getLightStructure().keyframes);
                        //light_layer->layernodes->m_node->layer_enabled = scene_file_xml.getLightStructure().enabled;
                        break;
                    }
                    case SceneFileXml::EFFECT :
                    {
                        scene_file_xml.setCurrentStringPosition(string_position + 1);
                        scene_file_xml.getEffectStructure().keyframes.keyframe_vector.clear();
                        scene_file_xml.parseEffectStructure( scene_file_xml.getEffectStructure() );
                        int plugin_id = -1;
                        EffectInfo::EFFECT_TYPE effect_type = EffectInfo::NOT_A_TYPE;
                        std::string guid = "";

                        if ( scene_file_xml.getEffectStructure().builtin.type != "NOT_A_TYPE")
                        {
                            effect_type = EffectInfo::getType(scene_file_xml.getEffectStructure().builtin.type);
                        }
                        else if ( scene_file_xml.getEffectStructure().plugin.guid != "" )
                        {
                            guid = scene_file_xml.getEffectStructure().plugin.guid;
                            plugin_id = m_pJPlugIn->getPluginId(guid);
                            effect_type = EffectInfo::JAH_PLUGIN_TYPE;
                        }

                        std::string* effect_name = &scene_file_xml.getEffectStructure().name;
                        EffectLayer* new_effect_layer = addEffectLayer(parent_layer, *effect_name, effect_type, plugin_id, guid);

                        if (new_effect_layer == NULL)
                        {
                            break;
                        }

                        if (module_name == "Keyer")
                        {
                            m_pChromaSettingsLayer = new_effect_layer->getJahLayer();
                        }

                        scene_file_xml.setKeyframes(new_effect_layer->getJahLayer(), scene_file_xml.getEffectStructure().keyframes);
                        parent_layer->setEffectsHaveKeyFrames(true);

                        if (new_effect_layer->getCategory() == EffectInfo::CPU_CATEGORY)
                        {
                            if (apply_effects_to_animation_layer && !new_jah_layer->containsCpuEffects() )
                            {
                                *new_jah_layer->getCompositeQimagePtr() = new_jah_layer->textureData.objectImage.copy();
                                *new_jah_layer->getCompositeQimageFlippedPtr() = 
                                    new_jah_layer->getCompositeQimagePtr()->mirror(false,true);
                            }

                            new_jah_layer->setContainsCpuEffects(true);
                        }

                        new_effect_layer->getJahLayer()->getCheckListItem()->setSelected(false);

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            } while (keyword != SceneFileXml::LAYER && string_position != int(std::string::npos) );

            //TO DO
            //we should only add the asset if the object has a asset!!!

          switch (layer_category)
            {
                case LayerCategory::LAYER :
                case LayerCategory::TEXT :
                case LayerCategory::OBJECT :
                case LayerCategory::PARTICLE :
                {
                    new_jah_layer->asset = asset_data;

                    new_jah_layer->layerAssetStatus = scene_file_xml.getAssetStructure().layer_status;
                    new_jah_layer->keyAssetStatus = scene_file_xml.getKeyAssetStructure().layer_status;

                    loadLayer(new_jah_layer, layer_name, layer_category, object_category, 
                        scene_file_xml.getAssetStructure().layer_status, asset_data, 
                        scene_file_xml.getKeyAssetStructure().layer_status, key_asset_data); 

                    if (getLayerList()->count() == 1 && scene_file_xml.getModuleStructure().name == "Animation")
                    {
                        m_pCamera = new_jah_layer;
                    }

                    if ( new_jah_layer->containsCpuEffects() )
                    {
                        *new_jah_layer->getCompositeQimagePtr() = new_jah_layer->textureData.objectImage.copy();
                        *new_jah_layer->getCompositeQimageFlippedPtr() = 
                            new_jah_layer->getCompositeQimagePtr()->mirror(false,true);
                    }

                    new_jah_layer->setEffectsSlidersHaveChanged(true);
                    QListViewItem* list_view_item = new_jah_layer->getCheckListItem();
                    m_pLayerListView->clearSelection();
                    m_pLayerListView->setCurrentItem(list_view_item);
                    m_pLayerListView->setSelected(list_view_item, true);

                    if (module_name == "Color")
                    {
                        setImageLayer(new_jah_layer);
                        changeSelection( getLayerList()->count() - 1 );
                    }
                    else if (module_name == "Keyer")
                    {
                         if (m_pChromaBaseLayer == NULL)
                         {
                            m_pChromaBaseLayer = new_jah_layer;
                         }
                         else
                         {
                            m_pChromaKeyLayer = new_jah_layer;
                         }
                    }
                    else if (module_name == "Tracker")
                    {
                        m_pBaseLayer = new_jah_layer;
                        createTrackerPointLayers();
                    }
                    else
                    {
                        layerClicked(list_view_item);
                    }

                    break;
                }
                case LayerCategory::TRACKER_POINT :
                {
                    int index = 0;

                    if (new_jah_layer->layername == "Red point")
                    {
                        index = 0;
                    }
                    else if (new_jah_layer->layername == "Green point")
                    {
                        index = 1;
                    }
                    else if (new_jah_layer->layername == "Grey point")
                    {
                        index = 2;
                    }
                    else if (new_jah_layer->layername == "Yellow point")
                    {
                        index = 3;
                    }

                    JahLayer* point_layer = getPointLayer(index);
                    motionNode* copy_to_keyframe = point_layer->layernodes->m_node;
                    motionNode* copy_from_keyframe = new_jah_layer->layernodes->m_node;
                    copy_to_keyframe->copy(copy_from_keyframe);
                    updateSliders(copy_to_keyframe);
                    setActiveJahLayer(m_pBaseLayer);
                    delete new_jah_layer->getCheckListItem();
                    getLayerList()->removeLast();
                }
                default:
                {
                    break;
                }
            }

        }

        //////////////////////////////////////////////////////
        //close out loading screen
        loadingscreen->close();
        updateKeyframeDisplay();
    }

    ////////////////////////////////////////////////
    //set the module to the first frame and refresh
    updateAnimation(1);
    updateGL();

    return true ;
}




